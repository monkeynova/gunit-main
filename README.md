Gunit Main
==========

There are a number of different Google C++ libraries out there in various states
of support for different flag models. This makes it a bit tricky to get them
all to work together for a main.

This reporistory provides a main suitable for bringing together absl flags,
benchmarking, logging (and gflags) as well as testing. It also provides the
library call that initialies all of these flags to be able to roll a different
main implementation.

```
WORKSPACE
...
git_repository(
    name = "com_monkeynova_gunit_main",
    remote = "git://github.com/monkeynova/gunit-main.git",
    ...
)
```

```
BUILD
...
cc_test(
  ...
  deps = [
    ...
	"@com_monkeynova_gunit_main//:test_main",
    ]
)
```
