use std::{
    fs::File,
    io::{self},
    path::{Path},
    process::Command,
};

const PROGRAM_PATH: &str = "../minishell";
const TESTS_PATH: &str = "tests.csv";
const ENABLE_BONUSES: bool = false;
const BLACKLIST: &[usize] = &[2, 3, 24, 68, 92, 102, 103, 405, 407, 418, 424, 425, 427, 734];

struct Test {
    id: usize,
    commands: String,
}

fn fix_tests(path: &Path) -> io::Result<()> {
    let string = std::fs::read_to_string(path)?
        .replace("(touche entrée)", "")
        .replace("[que des espaces]", "           ")
        .replace("[que des tabulations]", "\t\t\t\t\t\t\t\t")
        .replace("$UID", "$SHELL")
        .replace(" [$TERM],", " \"[$TERM]\",")
        .replace("sleep 3", "sleep 1");
    std::fs::write(path, string)?;
    Ok(())
}

fn parse_tests(path: &Path) -> io::Result<Vec<Test>> {
    let file = File::open(path)?;
    let mut ignored_tests = 0;
    let mut reader = csv::Reader::from_reader(file);
    let mut tests = vec![];
    for (id, result) in reader.records().skip(24).enumerate() {
        if BLACKLIST.contains(&id) {
            ignored_tests += 1;
            continue;
        }
        let record = result?;
        if !ENABLE_BONUSES {
            match record.get(2) {
                Some(str) if !str.is_empty() => {
                    ignored_tests += 1;
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
                || commands.contains("<<")
                || (!ENABLE_BONUSES && (commands.contains("&&") || commands.contains("||")))
            {
                ignored_tests += 1;
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
                ignored_tests += 1;
                continue;
            }
            commands
        } else {
            ignored_tests += 1;
            continue;
        };
        tests.push(Test { id, commands });
    }
    println!();
    println!("!!!   {ignored_tests} IGNORED TESTS   !!!");
    Ok(tests)
}

fn clear_dir(dir: &Path) -> io::Result<()> {
    if !dir.is_dir() {
        return Err(io::Error::new(
            io::ErrorKind::InvalidInput,
            "path is not a directory",
        ));
    }
    for entry in std::fs::read_dir(dir)? {
        let entry = entry?;
        let path = entry.path();
        if path.is_dir() && !path.symlink_metadata()?.file_type().is_symlink() {
            std::fs::remove_dir_all(&path)?;
        } else {
            std::fs::remove_file(&path)?;
        }
    }
    Ok(())
}

fn exec_test(test: &Test, program_path: &Path) -> io::Result<bool> {
    let current_dir = std::env::current_dir()?;
    println!();
    println!("##### TEST {:>7} #####", test.id);
    println!("{}", test.commands);
    clear_dir(&current_dir)?;
    let bash = match Command::new("bash").args(["-c", &test.commands]).output() {
        Ok(minishell) => minishell,
        Err(_) => {
            println!("##### INVALID TEST #####");
            return Ok(false);
        }
    };
    clear_dir(&current_dir)?;
    let minishell = match Command::new(program_path)
        .args(["-c", &test.commands])
        .output()
    {
        Ok(minishell) => minishell,
        Err(_) => {
            println!("#### FAILED TO RUN! ####");
            return Ok(false);
        }
    };
    match (bash.status.code(), minishell.status.code()) {
        (Some(bash_code), Some(minishell_code)) => {
            if bash_code != minishell_code {
                println!("######## FAILED ########");
                println!("Expected status {bash_code}, got {minishell_code}");
                println!("{}", String::from_utf8_lossy(&minishell.stderr));
                println!("########################");
                return Ok(false);
            }
        }
        (None, _) => {
            println!("#### FAILED TO RUN! ####");
            return Ok(false);
        }
        (_, None) => {
            println!("### PROGRAM CRASHED! ###");
            return Ok(false);
        }
    }
    let bash_stdout = String::from_utf8_lossy(&bash.stdout);
    let minishell_stdout = String::from_utf8_lossy(&minishell.stdout);
    if bash_stdout != minishell_stdout {
        println!("######## FAILED ########");
        println!("Expected output:");
        println!("{bash_stdout}");
        println!("Tested output:");
        println!("{minishell_stdout}");
        println!("########################");
        return Ok(false);
    }
    println!("####### SUCCESS! #######");
    Ok(true)
}

fn main() -> io::Result<()> {
    let args = std::env::args().collect::<Vec<_>>();
    let start_at = args.get(1).map_or(0, |arg| arg.parse().unwrap_or(0));
    let path = std::env::current_dir()?;
    let tests_path = path.join(TESTS_PATH);
    let program_path = path.join(PROGRAM_PATH);
    fix_tests(&tests_path)?;
    std::fs::create_dir(path.join("tmp")).ok();
    std::env::set_current_dir(path.join("tmp"))?;
    for test in parse_tests(&tests_path)?
        .iter()
        .skip_while(|test| test.id < start_at)
    {
        if !exec_test(test, &program_path)? {
            return Ok(());
        }
    }
    std::fs::remove_dir_all(path.join("tmp"))?;
    Ok(())
}
