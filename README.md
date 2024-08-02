# What is OpenMD?

[![build](https://github.com/OpenMD/OpenMD/workflows/build/badge.svg)](https://github.com/OpenMD/OpenMD/actions?query=workflow%3Abuild) [![status](https://joss.theoj.org/papers/8841bf23a51ceaf3439f455219043855/status.svg)](https://joss.theoj.org/papers/8841bf23a51ceaf3439f455219043855)


OpenMD is an open source molecular dynamics engine which is capable of
efficiently simulating liquids, proteins, nanoparticles, interfaces,
and other complex systems using atom types with orientational degrees
of freedom (e.g. "sticky" atoms, point dipoles, and coarse-grained
assemblies). Proteins, zeolites, lipids, transition metals (bulk, flat
interfaces, and nanoparticles) have all been simulated using force
fields included with the code. OpenMD works on parallel computers
using the Message Passing Interface (MPI), and comes with a number of
analysis and utility programs that are easy to use and modify. An
OpenMD simulation is specified using a very simple meta-data language
that is easy to learn.

## Getting Started

Simulations are started in OpenMD using a single Molecular Dynamics (.omd)
file. These files must start with the `<OpenMD>` tag and must have two
sections:

  1) a C-based `<MetaData>` section, and

  2) a `<Snapshot>` block for initial coordinate and velocity information.

Detailed descriptions of the structures of these files are available
in the `docs` directory. Sample simulations are available in the
`samples` directory and [QUICK_START.md](QUICK_START.md) walks you through a
procedure for running and analyzing your first OpenMD simulation.

## Requirements

 1) A good C++17-compliant compiler. We've built and tested OpenMD on the
    following architecture & compiler combinations:

| Architecture                   |  CXX | Notes                                 |
|--------------------------------|:----:|---------------------------------------|
| macOS Sonoma (intel & arm)     | c++  | (Apple clang 15.3, Open MPI 5.0.3)    |
| Linux (Ubuntu 24.04 - x86\_64) | g++  | (GNU version 13.2  Open MPI 4.1.6)    |
| Linux (RHEL 9.4 - x86\_64)     | icpx | (Intel version 23.0, Intel MPI 2021.8)|

  OpenMD uses features in the C++ standard library and language features from
  C++17. Most (but not all) C++ compilers support these features.

 2) CMake, a cross-platform build system which is available at
    [cmake.org](http://www.cmake.org). Most Linux and some Unix distributions
    provide CMake as a standard package. If not, please download it,
    and make sure you get a recent version. Mac OS X users can either
    download the CMake installer or install it from the command line
    using macports.

 3) An implementation of MPI-2 is optional for the single processor
    version of OpenMD, but is required if you want OpenMD to run in
    parallel. We like OpenMPI. Other implementations of MPI-2 might
    work, but we haven't tried. You can get Open MPI here:
    [open-mpi.org](http://www.open-mpi.org/)

 4) Other required libraries that unlock some features of OpenMD:

      + Open Babel:  [openbabel.org](http://openbabel.org)
      + Qhull:       [www.qhull.org](http://www.qhull.org)
      + FFTW:        [www.fftw.org](http://www.fftw.org)
      + Doxygen:     [www.doxygen.org](http://www.doxygen.org)

 5) Some of the utility scripts depend on Python, NumPy, and SciPy.  
    These are common installations on most flavors of Unix and
    Mac OS X.

## Instructions

 1) Get, build, and test the required pieces above.
 2) mkdir build
 3) cd build
 4) cmake ..
 5) make
 6) umask 0022; sudo make install

That's it. For more information on building and configuring OpenMD, check out our [INSTALL.md](docs/INSTALL.md) instructions.

## Contributing

Please read [CONTRIBUTING.md](.github/CONTRIBUTING.md) for details on how you can become a contributor and the process for submitting pull requests to us.

## License

Copyright (c) 2004-2024, OpenMD. All rights reserved.

Licensed under the [BSD 3-Clause License](LICENSE).

