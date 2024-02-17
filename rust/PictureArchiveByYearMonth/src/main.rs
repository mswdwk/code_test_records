use chrono::{DateTime, Datelike, Utc};
use lazy_static::lazy_static;
use std::collections::{HashMap, HashSet};
use std::fs::DirEntry;
use std::path::{Path, PathBuf};
use std::{fs, io};

lazy_static! {
    // static ref IGNORE_DIRS : HashSet<_> = HashSet::from([".git".to_own(),".gitignore".to_string()]);
    static ref IGNORE_DIRS : HashSet<String> = [".git",".idea","target","2024"]
        .iter().map(|x|x.to_string()).collect();
    static ref IGNORE_FILES : HashSet<String> = [".gitignore","main.rs","Cargo.toml","Cargo.lock","pictureArchiveByYearMonth.exe"]
        .iter().map(|x|x.to_string()).collect();

   /* static ref ALL_FILES:  HashMap<String,i32 > = {
        let mut m = HashMap::new();
        m.insert("foo".to_string(),1);
        m
    };*/
}

// one possible implementation of walking a directory only visiting files
fn visit_dirs(dir: &Path, cb: &mut dyn FnMut(&DirEntry)) -> io::Result<()> {
    if !dir.is_dir() {
        return Ok(());
    }
    for entry in fs::read_dir(dir)? {
        let entry = entry?;
        let path = entry.path();
        let pname = path.file_name().unwrap().to_str().unwrap();
        if path.is_dir() && !IGNORE_DIRS.contains(pname) {
            visit_dirs(&path, cb)?;
        } else if path.is_file() && !IGNORE_FILES.contains(pname) {
            cb(&entry);
        }
    }
    Ok(())
}

fn main() -> io::Result<()> {
    let mut all_files: HashMap<String, PathBuf> = HashMap::new();
    let mut entries = fs::read_dir(".")?
        .map(|res| res.map(|e| e.path()))
        .collect::<Result<Vec<_>, io::Error>>()?;
    // The order in which `read_dir` returns entries is not guaranteed. If reproducible
    // ordering is required the entries should be explicitly sorted.

    entries.sort();
    println!("file number is {} ", entries.len());
    let mut cb = |d: &DirEntry| {
        let meta = d.metadata().unwrap();
        let modify_time = meta.modified().unwrap();
        let dt: DateTime<Utc> = modify_time.into();

        let dest_dir = d.path().parent().unwrap().join(dt.year().to_string());

        let dest_path_name = dest_dir.join(d.file_name());

        if !dest_dir.exists() {
            if let Err(e) = fs::create_dir_all(dest_dir.as_os_str()) {
                println!(
                    "create dest dir '{:?}'failed: err= {:?}",
                    dest_dir,
                    e.to_string()
                );
                std::process::exit(1);
            }
            println!(
                " name {:?} size {:?} dt {:?} {:?} create dest_dir {:?} ok",
                d.file_name(),
                meta.len(),
                dt.year(),
                dt.month(),
                dest_dir.as_os_str()
            );
        }
        if dest_path_name.exists() {
            println!("already exists {:?}", dest_path_name.as_os_str());
            return;
        }
        if let Err(e) = fs::rename(d.path(), dest_path_name.as_os_str()) {
            println!(
                "rename file failed! old path {:?} dest_path_name {:?}, error is {}",
                d.path(),
                dest_path_name,
                e.to_string()
            );
        }
        // all_files.insert(d.file_name().into_string().unwrap(),dest_path_name);
        // TODO: CHECK if or not file is repeated
        let file_name = d.file_name().into_string().unwrap();
        if !all_files.contains_key(&file_name) {
            all_files.insert(file_name, dest_path_name);
        } else {
            println!(
                "repeated file {}, already path is {:?} ,added path is {:?}",
                file_name,
                all_files.get(&file_name).unwrap().as_os_str(),
                d.path().as_os_str()
            );
        }
        // std::process::exit(0);
    };

    let p = Path::new(".");
    let _ = visit_dirs(&p, &mut cb);
    println!("all_files number is  {}", all_files.len());
    Ok(())
}
