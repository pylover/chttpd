# chttpd
Nonblocking IO HTTP server using [caio](https://github.com/pylover/caio).


#### Dependencies

- [caio](https://github.com/pylover/caio)
- [clog](https://github.com/pylover/clog)
- [mrb](https://github.com/pylover/mrb)
- [cutest](https://github.com/pylover/cutest)

```bash
sudo apt install cmake cmake-curses-gui build-essential valgrind
```


## Build
```bash
mkdir build
cd build
cmake ..
make menu
make
```


### Run
```bash
./chttpd
```


## Contribution

### Lint

```bash
pip install prettyc
cd path/to/chttpd/build
make lint
```


## Tests

### Dependencies
- [cutest](https://github.com/pylover/cutest)

### Running tests
```bash
cd path/to/chttpd/build
make test
```
