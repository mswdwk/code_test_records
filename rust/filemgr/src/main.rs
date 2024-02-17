use std::{fs, io};
use std::path::Path;
use std::fs::DirEntry;
use chrono::{Datelike, DateTime, Utc};

// one possible implementation of walking a directory only visiting files
fn visit_dirs(dir: &Path, cb: &dyn Fn(&DirEntry)) -> io::Result<()> {
    if dir.is_dir() {
        for entry in fs::read_dir(dir)? {
            let entry = entry?;
            let path = entry.path();
            if path.is_dir() {
                visit_dirs(&path, cb)?;
            } else {
                cb(&entry);
            }
        }
    }
    Ok(())
}

fn main() -> io::Result<()> {
    let mut entries = fs::read_dir(".")?
        .map(|res| res.map(|e| e.path()))
        .collect::<Result<Vec<_>, io::Error>>()?;

    // The order in which `read_dir` returns entries is not guaranteed. If reproducible
    // ordering is required the entries should be explicitly sorted.

    entries.sort();
    let cb=  |d: &DirEntry|{
        let meta = d.metadata().unwrap();
        let modify_time = meta.modified().unwrap();
        let dt: DateTime<Utc> = modify_time.into();

        println!(" name {:?} size {:?} dt {:?} {} {} file_type {:?} path {:?} ",d.file_name(),meta.len(),
            dt.year(),dt.month(),dt.month0(), d.file_type().unwrap(), d.path().parent().unwrap());
    };
    for e in entries {
        println!("path: {} is_dir: {}",e.display(),e.is_dir());
    }
    let p = Path::new(".");
    return visit_dirs(&p,&cb);
    // Ok(())
}