# MediaWiki

## Introduction

This framework provides access to the API of MediaWiki sites.

## Dependencies

* CMake
* Qt
* KCoreAddons

## Install

    mkdir build && cd build
    cmake ..
    make
    sudo make install

## Run samples

    cd examples/editsample
    cmake .
    uic mainwindow.ui > ui_mainwindow.h
    make
    ./EditSample

## Reporting a bug

You can report a bug from:

- https://bugs.kde.org/enter_bug.cgi?product=libmediawiki

## Authors

See the AUTHORS file for details.

## Links

- Git repository: https://cgit.kde.org/libmediawiki.git