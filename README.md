# Semi-Auto File Sorter

A terminal-based C++ utility for manually navigating your file system and batch-moving files based on name keywords or file extensions.

---

## Features

- Navigate your file system interactively from the drive root
- Sort files by **name keyword** — moves all files whose name contains a given string
- Sort files by **file extension** — moves all files matching a given extension (e.g. `.txt`, `.jpg`)
- Auto-creates the destination directory if it doesn't exist
- Keeps a **session history** log of all move actions performed

---

## How to Use

### Main Menu

```
[ 1 ] Choose directory to sort
[ 2 ] View sorting history for this session
[ 3 ] Start sorting chosen directory
[ ~End~ ] Close the program
```

---

### Option 1 — Choose Directory

Navigates your file system starting from the drive root (`C:\` on Windows). Only folders are shown.

| Command | Action |
|---|---|
| *(folder name)* | Enter that folder |
| `~Back~` | Go up one directory |
| `~Here~` | Confirm this as the directory to sort |

---

### Option 3 — Sort Files

Scans all **files** (not folders) in the chosen directory and loads them for sorting. You will then be asked for a **destination directory** — it will be created automatically if it doesn't exist.

**Sort methods:**

**[ 1 ] By name keyword**
Moves all files whose filename contains the text you enter.
```
> report
→ Moves: quarterly_report.xlsx, report_final.docx, ...
```

**[ 2 ] By file extension**
Moves all files matching the exact extension you enter.
```
> .png
→ Moves: banner.png, icon.png, screenshot.png, ...
```

---

## Building

Requires **C++17** or later for `std::filesystem` support.

**g++:**
```bash
g++ -static -static-libgcc -static-libstdc++ "Semi-Auto File Sorter.cpp" -o "Semi-Auto File Sorter.exe"
```

---

## Notes

- The program starts navigation at your **drive root** (e.g. `C:\`), not the directory the executable is in
- Only **direct children** of the chosen directory are sorted — it does not sort recursively
- Extensions must be entered with the dot included (e.g. `.txt` not `txt`)
- The session history (Option 2) only persists for the current run — it resets on exit