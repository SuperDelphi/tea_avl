# tea_avl

## Group members

- SuperDelphi
- Loukas35
- WassimBKL
- ValB23

## Installing in Linux/WSL

```bash
git clone
mkdir debug
cd tea_avl/debug
cmake ../src/ -DCAME_INSTALL_PREFIX=../tmp -DCMAKE_BUILD-TYPE=Debug
make
```

## Test
In the debug folder, run
```bash
./test-tree-avl
```

TODO:
- [ ] Rotation functions (left/right)
- [ ] Insert function
- [ ] Delete function
- [ ] Search function