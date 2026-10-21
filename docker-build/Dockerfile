# Ubuntu 24.04 is the supported Linux wheel baseline (glibc 2.39).
FROM ubuntu:24.04 AS build
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y --no-install-recommends \
    ca-certificates build-essential cmake libboost-dev python3-dev python3-venv swig patchelf \
    && rm -rf /var/lib/apt/lists/*
RUN python3 -m venv /opt/build-venv
ENV PATH="/opt/build-venv/bin:$PATH"
# The fixed digest authenticates pip even if the original trust store is stale.
RUN python -m pip --disable-pip-version-check install --no-deps --upgrade "https://files.pythonhosted.org/packages/3f/06/3e4420302363fbb71be3c7f35ce042f54bfca330e3e66b3a2e3c4cca579f/pip-26.2.2-py3-none-any.whl#sha256=fd7f4dfa8190e014013b9f08f3e973f4e4c26ffd179a204895533a7254813a70"
# Use patched bundled roots rather than inheriting stale native trust.
ENV PIP_USE_DEPRECATED=legacy-certs PIP_DISABLE_PIP_VERSION_CHECK=1
RUN python -m pip install "certifi>=2026.10.19" scikit-build setuptools wheel auditwheel
WORKDIR /build
COPY siqadtools-*.tar.gz /tmp/siqadtools.tar.gz
RUN tar --strip-components=1 -xzf /tmp/siqadtools.tar.gz
RUN cmake -S . -B /tmp/cpp-tests -DBUILD_TESTING=ON \
    && cmake --build /tmp/cpp-tests --parallel 2 \
    && ctest --test-dir /tmp/cpp-tests --output-on-failure
RUN python setup.py bdist_wheel \
    && auditwheel repair --plat manylinux_2_39_$(uname -m) dist/*.whl -w /wheelhouse
RUN python -m pip install --no-index --no-deps /wheelhouse/*.whl \
    && python -c "from siqadtools import siqadconn"

FROM scratch AS wheels
COPY --from=build /wheelhouse/ /dist/
