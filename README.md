# SiQADConnector

SiQADConnector is a class containing convenient functions for physics engines to interact with the SiQAD GUI. The use of the class is recommended, but ultimately optional as developers may want to implement their own I/O with SiQAD. A Python wrapper generated using [SWIG](http://www.swig.org/) is also been included. Note that SiQADConnector uses Apache License 2.0 as opposed to the GUI's LGPLv3 in order to allow physics engines to use any license compatible with APLv2.

Please keep in mind that SiQAD is still undergoing rapid development. Compatibility is not guaranteed across SiQADConnector and SiQAD versions.


## Installation for Python Projects

### Pre-compiled Wheels

We compile wheels targeting Python versions 3.5 to 3.8 on Linux and Windows. Compiled binaries for macOS are planned for the future.

Currently, the easiest way to acquire SiQADConnector is through PIP (or your preferred Python package manager) from our testing repository:

```
# Substitute pip with the appropriate command for your platform, e.g. pip3, python3 -m pip, etc.
pip install -i https://test.pypi.org/simple/ siqadtools
```

We will eventually move this from [TestPyPI](https://test.pypi.org/) to the proper [PyPI](https://pypi.org/).

You may also download compiled wheels here: TODO add link, and install them through PIP or your preferred Python package manager.

To uninstall, run
```
# Substitute pip with the appropriate command for your platform
pip uninstall siqadtools
```

### Compiling the Wheel from Source

#### Ubuntu 18.04 LTS

Install dependencies (packages listed are for Ubuntu 18.04 LTS, please adapt to your distribution):

```
apt-get install swig libboost libboost-dev cmake
pip3 install scikit-build
```

Run the `setup.py` script:

```
python3 setup.py bdist_wheel
```

Here, `bdist_wheel` instructs the script to package the compiled package into a wheel for you to install. The compiled wheel is deposited into the `dist` subdirectory from where you run the script. Install the wheel on your system using your preferred Python package manager, e.g. `pip`.

To uninstall, run
```
pip3 uninstall siqadtools
```

##### Windows

We use AppVeyor, a CI service which has Windows build support, to compile SiQADConnector wheels for distribution on the Windows system. Please refer to our [AppVeyor configuration file](appveyor.yml) for our configurations and setup, and adapt to your environment.




## Usage

### C++ projects

Either copy `siqadconn.cc` and `siqadconn.h` to your project directory and include `siqadconn.h` appropriately, or add this repository to your project as a submodule and include `siqadconn.h` as needed.

Usage examples will be included in the future. In the meantime, please refer to [SimAnneal's usage](https://github.com/samuelngsh/simanneal-sidb/blob/master/interface.cc) for some examples.

### Python projects

Import SiQADConnector by:

```
from siqadtools import siqadconn
```

See [ExhaustiveGS](https://github.com/samuelngsh/exhaustive-gs-sidb) and [PoisSolvers](https://github.com/NathanChiu/PoisSolvers) for some usage examples of SiQADConnector in Python projects.

