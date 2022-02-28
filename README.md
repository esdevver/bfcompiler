# bfcompiler
Compiler for BrainF

![GitHub](https://img.shields.io/github/license/esdevver/bfcompiler)
![GitHub repo size](https://img.shields.io/github/repo-size/esdevver/bfcompiler)

![GitHub last commit](https://img.shields.io/github/last-commit/esdevver/bfcompiler)
![GitHub commit activity](https://img.shields.io/github/commit-activity/m/esdevver/bfcompiler)

### Todo List
- [x] Translate source into AST
- [ ] Translate AST into IR
- [ ] Optimise IR
- [ ] Translate IR into other languages
- - [ ] C
- - [ ] Python
- - [ ] JavaScript

### Building

To build with plain output
```bash
make clean ; make
```

To build with color output
```bash
make clean ; CFLAGS=-DCOLOR_OUTPUT make
```

To build source files together without intermediate object files
```bash
make clean ; make hefty
```
