use std::{
    fs::File,
    io::{self},
    path::PathBuf,
    process::Command,
};

const TESTS_PATH: &str = "tests.csv";
const ENABLE_BONUSES: bool = false;

struct Test {
    id: usize,
    commands: String,
    expected: String,
    exit_code: i32,
}

fn fix_tests(path: PathBuf) -> io::Result<()> {
    let mut string = std::fs::read_to_string(&path)?;
    string = string
        .replace("(touche entrée)", "")
        .replace("[que des espaces]", "           ")
        .replace("[que des tabulations]", "\t\t\t\t\t\t\t\t");
    std::fs::write(&path, string)?;
    Ok(())
}

fn parse_tests(path: PathBuf) -> io::Result<Vec<Test>> {
    let file = File::open(path)?;
    let mut reader = csv::Reader::from_reader(file);
    let mut tests = vec![];
    for (id, result) in reader.records().skip(24).enumerate() {
        let record = result?;
        if !ENABLE_BONUSES {
            match record.get(2) {
                Some(str) if !str.is_empty() => {
                    continue;
                }
                _ => (),
            }
        }
        let commands = if let Some(commands) = record.get(1) {
            let lines = commands.lines().map(|line| line.strip_prefix("$> "));
            if lines.clone().any(|line| line.is_none()) {
                println!("INVALID TEST :");
                println!("{commands}");
                println!();
                continue;
            }
            lines.map(Option::unwrap).collect()
            // .filter_map(|line| {
            //     let mut words = line.split(' ');
            //     let cmd = words.next()?;
            //     let args = words.collect::<Vec<_>>();
            //     let mut command = Command::new(cmd);
            //     command.args(args);
            //     Some(command)
            // })
            // .collect::<Vec<_>>()
        } else {
            continue;
        };
        let expected = if let Some(expected) = record.get(7) {
            expected.to_owned()
        } else {
            continue;
        };
        let exit_code = if let Some(Ok(exit_code)) = record.get(8).map(str::parse) {
            exit_code
        } else {
            continue;
        };
        tests.push(Test {
            id,
            commands,
            expected,
            exit_code,
        });
    }
    Ok(tests)
}

fn exec_test(test: &Test) -> bool {
    println!();
    println!("##### TEST {:>7} #####", test.id);
    println!("{}", test.commands);
    if Command::new("bash")
        .args(["-c", &test.commands])
        .output()
        .is_err()
    {
        println!("##### INVALID TEST #####");
        return false;
    }
    let minishell = match Command::new("../minishell")
        .args(["-c", &test.commands])
        .output()
    {
        Ok(minishell) => minishell,
        Err(_) => {
            println!("#### FAILED TO RUN! ####");
            return false;
        }
    };
    match minishell.status.code() {
        Some(code) => {
            if code != test.exit_code {
                println!("######## FAILED ########");
                println!(
                    "Expected status {}, got {}",
                    test.exit_code,
                    minishell.status.code().unwrap()
                );
                println!("########################");
                return false;
            }
        }
        None => {
            println!("### PROGRAM CRASHED! ###");
            return false;
        }
    }
    for (minishell_byte, test_byte) in minishell.stdout.iter().zip(test.expected.bytes()) {
        if *minishell_byte != test_byte {
            println!("######## FAILED ########");
            println!("Expected output:");
            println!("{}", test.expected);
            println!("Tested output:");
            println!("{}", String::from_utf8(minishell.stdout).unwrap());
            println!("########################");
            return false;
        }
    }
    println!("####### SUCCESS! #######");
    true
}

fn main() -> io::Result<()> {
    fix_tests(TESTS_PATH.into())?;
    for test in parse_tests(TESTS_PATH.into())?.iter() {
        if !exec_test(test) {
            // break;
        }
        let mut str = String::new();
        io::stdin().read_line(&mut str)?;
    }
    Ok(())
}
