#!/usr/bin/env python3
"""
Batch-update JSON paths for *_anim.json and *_sprites.json.

- *_anim.json (top-level object):
  - Controlled by --anim-mode {expand|strip}
    * expand (default): filePath = "{TOP}/{Subfolder}/{Name}.png"
    * strip           : filePath = "{Name}.png"

- *_sprites.json (TOP-LEVEL "name" ONLY):
  - Controlled by --sprites-mode {expand|strip}
    * expand (default): "JumpDown_E.png" -> "PlayerAnimation/JumpDown/JumpDown_E.png"
    * strip           : "PlayerAnimation/JumpDown/JumpDown_E.png" -> "JumpDown_E.png"

General:
- Works recursively under ROOT.
- Writes UTF-8 with indent=4. (❗ 파일 끝 개행을 추가하지 않습니다)
- Creates .bak backups unless --no-backup is specified.
- Use --dry-run to preview.

Also:
- More tolerant JSON loader: if a file contains extra trailing JSON/text,
  we auto-detect the end of the first top-level object and parse only that.
"""

import argparse
import json
import sys
from pathlib import Path
from typing import Any, Dict, List, Tuple
import re

PNG_RE = re.compile(r'(?i)\.png$')


# -------------------- robust JSON loader --------------------

def _find_first_object_end(text: str) -> int:
    """Return index just after the first complete top-level JSON object, or -1 if not found."""
    depth = 0
    in_str = False
    esc = False
    started = False

    for i, ch in enumerate(text):
        if in_str:
            if esc:
                esc = False
            elif ch == '\\':
                esc = True
            elif ch == '"':
                in_str = False
            continue

        # not in string
        if ch == '"':
            in_str = True
        elif ch == '{':
            depth += 1
            started = True
        elif ch == '}':
            if depth > 0:
                depth -= 1
                if depth == 0 and started:
                    return i + 1
        # ignore other chars
    return -1


def load_json_lenient(path: Path) -> Tuple[Any, str]:
    """
    Attempt to load JSON from file.
    - Trims UTF-8 BOM if present.
    - If 'Extra data' occurs, parse only the first top-level object.
    Returns: (obj, note)  where note is "" or e.g. "[CLEANED: extra trailing data removed]"
    """
    raw = path.read_text(encoding="utf-8-sig")
    try:
        return json.loads(raw), ""
    except json.JSONDecodeError as e:
        msg = str(e)
        if "Extra data" in msg:
            end = _find_first_object_end(raw)
            if end > 0:
                head = raw[:end]
                try:
                    obj = json.loads(head)
                    return obj, "[CLEANED: extra trailing data removed]"
                except Exception:
                    pass
        # If still failing, re-raise with context
        raise


# -------------------- helpers --------------------

def split_action_and_dir(name: str) -> Tuple[str, str]:
    """'JumpDown_E' -> ('JumpDown', 'E') using the LAST underscore."""
    if '_' not in name:
        return name, ''
    base, dir_tag = name.rsplit('_', 1)
    return base, dir_tag

def derive_subfolder_from_name(name: str) -> str:
    base, _ = split_action_and_dir(name)
    return base

def ensure_parent_backup(path: Path, no_backup: bool):
    if no_backup:
        return
    bak = path.with_suffix(path.suffix + ".bak")
    if not bak.exists():
        bak.write_bytes(path.read_bytes())

def write_json_no_trailing_newline(path: Path, data: Any):
    # 저장 시 파일 끝 개행 없음
    text = json.dumps(data, ensure_ascii=False, indent=4)
    path.write_text(text, encoding="utf-8")

def compute_target_path(top_name: str, action_or_subfolder: str, name_with_dir_png: str) -> str:
    return f"{top_name}/{action_or_subfolder}/{name_with_dir_png}".replace("\\", "/")


# -------------------- anim updaters --------------------

def update_anim_file_expand(path: Path, top_name: str, dry_run: bool, no_backup: bool) -> Tuple[bool, str]:
    try:
        obj, note = load_json_lenient(path)
    except Exception as e:
        return False, f"[SKIP] {path}: JSON parse error: {e}"

    if not isinstance(obj, dict):
        return False, f"[SKIP] {path}: Expected a JSON object."

    name = obj.get("name")
    file_path_current = obj.get("filePath")
    if not isinstance(name, str):
        return False, f"[SKIP] {path}: Missing or non-string 'name'."

    subfolder = derive_subfolder_from_name(name)
    target_filename = f"{name}.png"
    target_path = compute_target_path(top_name, subfolder, target_filename)

    if file_path_current == target_path and not note:
        return False, f"[OK ] {path}: filePath already expanded."

    obj["filePath"] = target_path
    if dry_run:
        return True, f"[DRY] {path}: {('NOTE ' + note + ' ') if note else ''}filePath: {file_path_current!r} -> {target_path!r}"
    ensure_parent_backup(path, no_backup)
    write_json_no_trailing_newline(path, obj)
    return True, f"[OK ] {path}: {('NOTE ' + note + ' ') if note else ''}filePath expanded."

def update_anim_file_strip(path: Path, dry_run: bool, no_backup: bool) -> Tuple[bool, str]:
    try:
        obj, note = load_json_lenient(path)
    except Exception as e:
        return False, f"[SKIP] {path}: JSON parse error: {e}"

    if not isinstance(obj, dict):
        return False, f"[SKIP] {path}: Expected a JSON object."

    name = obj.get("name")
    file_path_current = obj.get("filePath")
    if not isinstance(name, str):
        return False, f"[SKIP] {path}: Missing or non-string 'name'."

    target_path = f"{name}.png"
    if file_path_current == target_path and not note:
        return False, f"[OK ] {path}: filePath already stripped."

    obj["filePath"] = target_path
    if dry_run:
        return True, f"[DRY] {path}: {('NOTE ' + note + ' ') if note else ''}filePath: {file_path_current!r} -> {target_path!r}"
    ensure_parent_backup(path, no_backup)
    write_json_no_trailing_newline(path, obj)
    return True, f"[OK ] {path}: {('NOTE ' + note + ' ') if note else ''}filePath stripped."


# -------------------- sprites updaters (top-level only) --------------------

def extract_filename(value: str) -> str:
    v = value.replace("\\", "/")
    return v.rsplit("/", 1)[-1]

def expand_sprites_top_name(path: Path, top_name: str, dry_run: bool, no_backup: bool) -> Tuple[bool, str]:
    try:
        data, note = load_json_lenient(path)
    except Exception as e:
        return False, f"[SKIP] {path}: JSON parse error: {e}"

    if not isinstance(data, dict) or "name" not in data or not isinstance(data["name"], str):
        return False, f"[SKIP] {path}: Top-level 'name' string not found."

    original = data["name"]
    fname = extract_filename(original)
    if not PNG_RE.search(fname):
        return False, f"[OK ] {path}: Top-level 'name' is not a PNG; unchanged."

    stem = fname[:-4]
    subfolder = derive_subfolder_from_name(stem)
    target = compute_target_path(top_name, subfolder, fname)

    if original == target and not note:
        return False, f"[OK ] {path}: Top-level 'name' already expanded."

    data["name"] = target
    if dry_run:
        return True, f"[DRY] {path}: {('NOTE ' + note + ' ') if note else ''}name: {original!r} -> {target!r}"
    ensure_parent_backup(path, no_backup)
    write_json_no_trailing_newline(path, data)
    return True, f"[OK ] {path}: {('NOTE ' + note + ' ') if note else ''}Top-level 'name' expanded."

def strip_sprites_top_name(path: Path, dry_run: bool, no_backup: bool) -> Tuple[bool, str]:
    try:
        data, note = load_json_lenient(path)
    except Exception as e:
        return False, f"[SKIP] {path}: JSON parse error: {e}"

    if not isinstance(data, dict) or "name" not in data or not isinstance(data["name"], str):
        return False, f"[SKIP] {path}: Top-level 'name' string not found."

    original = data["name"]
    fname = extract_filename(original)
    if original == fname and not note:
        return False, f"[OK ] {path}: Top-level 'name' already stripped."

    data["name"] = fname
    if dry_run:
        return True, f"[DRY] {path}: {('NOTE ' + note + ' ') if note else ''}name: {original!r} -> {fname!r}"
    ensure_parent_backup(path, no_backup)
    write_json_no_trailing_newline(path, data)
    return True, f"[OK ] {path}: {('NOTE ' + note + ' ') if note else ''}Top-level 'name' stripped."


# -------------------- main --------------------

def main():
    parser = argparse.ArgumentParser(description="Batch-fix anim/sprites JSON paths.")
    parser.add_argument("root", type=str, help="Root folder (e.g., the top-level 'PlayerAnimation' directory).")
    parser.add_argument("--top-name", type=str, default=None,
                        help="Top-level folder name for expand mode (default: basename of ROOT).")
    parser.add_argument("--anim-mode", choices=["expand", "strip"], default="expand",
                        help="How to handle 'filePath' in *_anim.json (default: expand).")
    parser.add_argument("--sprites-mode", choices=["expand", "strip"], default="expand",
                        help="How to handle TOP-LEVEL 'name' in *_sprites.json (default: expand).")
    parser.add_argument("--dry-run", action="store_true", help="Preview changes without writing files.")
    parser.add_argument("--no-backup", action="store_true", help="Do not create .bak backups.")
    args = parser.parse_args()

    root = Path(args.root).resolve()
    if not root.exists() or not root.is_dir():
        print(f"[ERR] Root folder not found: {root}", file=sys.stderr)
        sys.exit(2)

    top_name = args.top_name or root.name

    anim_paths: List[Path] = []
    sprites_paths: List[Path] = []

    for p in root.rglob("*.json"):
        name = p.name.lower()
        if name.endswith("_anim.json"):
            anim_paths.append(p)
        elif name.endswith("_sprites.json"):
            sprites_paths.append(p)

    total_changed = 0
    print(f"Top name       : {top_name}")
    print(f"Anim mode      : {args.anim_mode}")
    print(f"Sprites mode   : {args.sprites_mode}")
    print(f"Root           : {root}")
    print(f"Anim files     : {len(anim_paths)}")
    print(f"Sprites files  : {len(sprites_paths)}")
    print("-" * 60)

    # Anim
    for anim in sorted(anim_paths):
        if args.anim_mode == "expand":
            changed, msg = update_anim_file_expand(anim, top_name, args.dry_run, args.no_backup)
        else:
            changed, msg = update_anim_file_strip(anim, args.dry_run, args.no_backup)
        print(msg)
        if changed:
            total_changed += 1

    # Sprites (top-level only)
    for spr in sorted(sprites_paths):
        if args.sprites_mode == "expand":
            changed, msg = expand_sprites_top_name(spr, top_name, args.dry_run, args.no_backup)
        else:
            changed, msg = strip_sprites_top_name(spr, args.dry_run, args.no_backup)
        print(msg)
        if changed:
            total_changed += 1

    print("-" * 60)
    print(f"Done. Files changed (or would change): {total_changed}")

if __name__ == "__main__":
    main()
