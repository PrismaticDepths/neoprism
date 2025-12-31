#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

void hello() {
	cout << 'hi' << '\n';
}

PYBIND11_MODULE(playback, m) {
    m.def("", &play_macro, "Play a macro from a byte array");
}
