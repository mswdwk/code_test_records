use std::{fs, io};
use std::path::Path;
use std::fs::DirEntry;

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
        println!(" name {:?}",d.file_name());
    };
    for e in entries {
        println!("path: {} is_dir: {}",e.display(),e.is_dir());
    }
    let p = Path::new(".");
    visit_dirs(&p,&cb);
    Ok(())
}