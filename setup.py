#!/usr/bin/env python

from skbuild import setup

setup (
        name    = 'siqadtools',
        version = '0.2.2',
        cmake_with_sdist = True,
        packages = ['siqadtools'],
        zip_safe = False
        author="Walus Lab",
        author_email="siqaddev@gmail.com",
        description="Python tools for the SiQAD project.",
        url="https://github.com/siqad/siqadconn",
        classifiers=[
            "Intended Audience :: Science/Research",
            "Programming Language :: C++",
            "Programming Language :: Python :: 3 :: Only",
            "Programming Language :: Python :: 3",
            "Programming Language :: Python :: 3.5",
            "Programming Language :: Python :: 3.6",
            "Programming Language :: Python :: 3.7",
            "Programming Language :: Python :: 3.8",
            "Topic :: Scientific/Engineering",
            "License :: OSI Approved :: Apache Software License",
            "Operating System :: POSIX :: Linux",
            "Operating System :: Microsoft :: Windows",
            ],
        python_requires='>=3.5',
        )
