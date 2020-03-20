Find data files in:

/Users/michael/Library/Application\ Support/CrossOver/Bottles/Broken\ Sword\ 2.5\ The\ Return\ Of\ The\ Templars/drive_c/Program\ Files/Broken\ Sword\ 2.5

Challenges to get sword25 working:

- data file is too large, we'd need to split it.
  See: https://emscripten.org/docs/api_reference/fetch.html#managing-large-files
  Maybe we can reference the files inside .data separately.

How the files are currenlty loaded:
- https://emscripten.org/docs/porting/files/packaging_files.html