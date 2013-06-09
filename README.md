# nix-password-checker

A very basic application to check if a user/password combination is correct.

This application reads `/etc/shadow` to find the hashed password, it can therefore only be ran as root.

## Dependencies

The app depends on [libconfig](http://www.hyperrealm.com/libconfig/) C++ bindings.

## How to use

Compile the application with

```
mkdir build
cd build
cmake ..
make
```

You can then run it by using

```
sudo ./src/nix-password-checker USERNAME PASSWORD
```

## Configuration

The application can be set to only allow access to certain groups or users.
This can be done with the following settings in `conf/application.cfg`.

```
application:
{
  authorized-groups = ["users"];
  authorized-users = ["alice", "bob"];
};
```

If you do not need this setting, you can delete the file, remove the keys, or leave an empty array.
