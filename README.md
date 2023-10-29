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

# Black & Red tree

The src file is used in the same way as the original src for the AVL tree.

## Installing in Linux/WSL for BR Tree
```bash
mkdir debug
cd tea_avl/debug
cmake ../src_br/ -DCAME_INSTALL_PREXIX=../tmp -DCMAKE_BUILD-TYPE=Debug
make
```

## Running the test file
In the debug file
```bash
./test-tree-br
```