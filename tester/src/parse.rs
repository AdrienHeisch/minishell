use crate::{Level, Test, BLACKLIST, BONUS_RANGES};
use std::{
    fs::File,
    io::{self},
    path::Path,
};

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

pub fn parse_tests(path: &Path, level: Level, skip_sleep: bool) -> io::Result<Vec<Test>> {
    fix_tests(path)?;
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
        if BONUS_RANGES.iter().any(|range| range.contains(&id)) {
            if level < Level::Bonus {
                ignored_tests += 1;
                continue;
            }
        } else if level < Level::More {
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
                || commands.contains(";")
                || (skip_sleep && commands.contains("sleep"))
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
