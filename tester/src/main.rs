use std::{
    fs::File,
    io::{self},
    path::PathBuf,
    process::Command,
};

struct Test {
    commands: String,
    expected: String,
    exit_code: i32,
}

fn parse_tests(path: PathBuf) -> io::Result<Vec<Test>> {
    let file = File::open(path)?;
    let mut reader = csv::Reader::from_reader(file);
    let mut tests = vec![];
    for result in reader.records().skip(24) {
        let record = result?;
        let commands = if let Some(commands) = record.get(1) {
            commands.into()
            // .lines()
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
            commands,
            expected,
            exit_code,
        });
    }
    Ok(tests)
}

fn exec_test(test: &Test) {
    let bash = Command::new("bash").args(["-c", &test.commands]);
    let minishell = Command::new("../minishell").args(["-c", &test.commands]);
}

fn main() -> io::Result<()> {
    parse_tests("tests.csv".into())?.iter().for_each(exec_test);
    Ok(())
}
