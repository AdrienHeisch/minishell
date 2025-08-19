use std::{
    fs::File,
    io::{self},
    path::PathBuf,
    process::Command,
};

const PROGRAM_PATH: &str = "/home/adrien/Dev/ft/minishell/minishell";
const TESTS_PATH: &str = "tests.csv";
const ENABLE_BONUSES: bool = false;
const BLACKLIST: &[usize] = &[2, 3, 24, 68, 92, 102, 103, 407, 418, 424, 425, 427];

struct Test {
    id: usize,
    commands: String,
}

fn fix_tests(path: PathBuf) -> io::Result<()> {
    let string = std::fs::read_to_string(&path)?
        .replace("(touche entrée)", "")
        .replace("[que des espaces]", "           ")
        .replace("[que des tabulations]", "\t\t\t\t\t\t\t\t")
        .replace("$UID", "$SHELL")
        .replace(" [$TERM],", " \"[$TERM]\",")
        .replace("sleep 3", "sleep 1");
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
                || commands.contains("env")
                || commands.contains("export")
                || commands.contains("unset")
                || (!ENABLE_BONUSES && (commands.contains("&&") || commands.contains("||")))
            {
                continue;
            }
            let mut lines = Vec::new();
            for line in commands.lines() {
                let stripped = line.strip_prefix("$> ");
                match stripped {
                    Some(line) => lines.push(line.to_owned()),
                    None => match lines.last_mut() {
                        Some(prev) => prev.push_str(line),
                        None => {
                            is_valid = false;
                            break;
                        }
                    },
                }
            }
            let commands = lines.join("\n");
            if !is_valid {
                println!("INVALID TEST : {id}");
                if !commands.is_empty() {
                    println!("{commands}");
                }
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
    let minishell = match Command::new(PROGRAM_PATH)
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
                println!("{}", String::from_utf8(minishell.stderr).unwrap());
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
    let bash_stdout = String::from_utf8(bash.stdout).unwrap();
    let minishell_stdout = String::from_utf8_lossy(&minishell.stdout);
    if bash_stdout != minishell_stdout {
        println!("######## FAILED ########");
        println!("Expected output:");
        println!("{bash_stdout}");
        println!("Tested output:");
        println!("{minishell_stdout}");
        println!("########################");
        return false;
    }
    println!("####### SUCCESS! #######");
    true
}

fn main() -> io::Result<()> {
    let args = std::env::args().collect::<Vec<_>>();
    let start_at = args.get(1).map(|arg| arg.parse().unwrap_or(0)).unwrap_or(0);
    let path = std::env::current_dir()?;
    fix_tests(TESTS_PATH.into())?;
    std::fs::remove_dir_all(path.join("tmp")).ok();
    for test in parse_tests(TESTS_PATH.into())?
        .iter()
        .skip_while(|test| test.id < start_at)
    {
        std::fs::create_dir(path.join("tmp"))?;
        std::env::set_current_dir(path.join("tmp"))?;
        if !exec_test(test) {
            break;
        }
        std::env::set_current_dir(&path)?;
        std::fs::remove_dir_all(path.join("tmp"))?;
    }
    Ok(())
}
