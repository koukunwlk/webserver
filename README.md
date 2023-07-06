# webserver


## Tests

### How to install gtest ?

If you are running on a Debian based OS just clone this [Gist](https://gist.github.com/dlime/313f74fd23e4267c4a915086b84c7d3d) and follow the steps

### How to test?
Create a test file ended by spec.cpp and write your tests
to execute the test just execute ``make test_file FILENAME=<yourfile withou extension>``
exemple:
```bash
cd myClassFolder
touch MyClass.spec.cpp
```
write you tests, go to the root of repository and execute
```
make test_file FILENAME=MyClass
```

### How to run all tests
just execute
```bash
make test_all
```
