use std::{
    fs::File,
    io::{self},
    path::PathBuf,
    process::Command,
};

const TESTS_PATH: &str = "tests.csv";
const ENABLE_BONUSES: bool = false;
const BLACKLIST: &[usize] = &[2, 3, 24, 68, 92, 102, 103];

struct Test {
    id: usize,
    commands: String,
}

fn fix_tests(path: PathBuf) -> io::Result<()> {
    let mut string = std::fs::read_to_string(&path)?;
    string = string
        .replace("(touche entrée)", "")
        .replace("[que des espaces]", "           ")
        .replace("[que des tabulations]", "\t\t\t\t\t\t\t\t")
        .replace("$UID", "$SHELL");
    std::fs::write(&path, string)?;
    Ok(())
}

fn parse_tests(path: PathBuf) -> io::Result<Vec<Test>> {
    let file = File::open(path)?;
    let mut reader = csv::Reader::from_reader(file);
    let mut tests = vec![];
    for (id, result) in reader.records().skip(24).enumerate() {
        if BLACKLIST.contains(&id) {
            continue;
        }
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
            let mut is_valid = true;
            if commands.contains("Ctlr-")
                || (!ENABLE_BONUSES && (commands.contains("&&") || commands.contains("||")))
            {
                continue;
            }
            let commands = commands
                .lines()
                .map(|line| line.strip_prefix("$> "))
                .filter_map(|line| {
                    if line.is_none() {
                        is_valid = false;
                    }
                    line
                })
                .collect();
            if !is_valid {
                println!("INVALID TEST :");
                println!("{commands}");
                println!();
                continue;
            }
            commands
        } else {
            continue;
        };
        tests.push(Test { id, commands });
    }
    Ok(tests)
}

fn exec_test(test: &Test) -> bool {
    println!();
    println!("##### TEST {:>7} #####", test.id);
    println!("{}", test.commands);
    let bash = match Command::new("bash").args(["-c", &test.commands]).output() {
        Ok(minishell) => minishell,
        Err(_) => {
            println!("##### INVALID TEST #####");
            return false;
        }
    };
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
    match (bash.status.code(), minishell.status.code()) {
        (Some(bash_code), Some(minishell_code)) => {
            if bash_code != minishell_code {
                println!("######## FAILED ########");
                println!("Expected status {bash_code}, got {minishell_code}");
                println!("########################");
                return false;
            }
        }
        (None, _) => {
            println!("#### FAILED TO RUN! ####");
            return false;
        }
        (_, None) => {
            println!("### PROGRAM CRASHED! ###");
            return false;
        }
    }
    for (bash_byte, minishell_byte) in bash.stdout.iter().zip(minishell.stdout.iter()) {
        if bash_byte != minishell_byte {
            println!("######## FAILED ########");
            println!("Expected output:");
            println!("{}", String::from_utf8(bash.stdout).unwrap());
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
            break;
        }
        // let mut str = String::new();
        // io::stdin().read_line(&mut str)?;
    }
    Ok(())
}
