#include "printers.h"

static std::mutex s_Vector;

void print_hello_word(int i)
{
    PROFILE_FUNCTION();
    std::this_thread::sleep_for(std::chrono::seconds(i));
    std::cerr << "HOLAAAAAAAAAAAAAAAAAA " << i << "\n";
}

void print_fftwf_complex(const fftwf_complex *carray, int n)
{
    PROFILE_FUNCTION();
    for (int i = 0; i < 2; ++i)
    {
        std::cerr << "(";
        std::cerr << std::setprecision(15) << carray[i][REAL] << ", ";
        std::cerr << std::setprecision(15) << carray[i][IMAG] << ") ";
    }
    std::cerr << "..."
              << " ";
    for (int i = n - 2; i < n; ++i)
    {
        std::cerr << "(";
        std::cerr << std::setprecision(15) << carray[i][REAL] << ", ";
        std::cerr << std::setprecision(15) << carray[i][IMAG] << ") ";
    }
    std::cerr << "\n";
    // std::cerr << __INT_MAX__ << ' ' << __FLT_MAX_EXP__ << '\n';
}

void print_complex(const std::complex<float> *carray, int n)
{
    PROFILE_FUNCTION();
    for (int i = 0; i < 2; ++i)
    {
        std::cerr << "(";
        std::cerr << std::setprecision(15) << carray[i].real() << ", ";
        std::cerr << std::setprecision(15) << carray[i].imag() << ") ";
    }
    std::cerr << "..."
              << " ";
    for (int i = n - 2; i < n; ++i)
    {
        std::cerr << "(";
        std::cerr << std::setprecision(15) << carray[i].real() << ", ";
        std::cerr << std::setprecision(15) << carray[i].imag() << ") ";
    }
    std::cerr << "\n";
    // std::cerr << __INT_MAX__ << ' ' << __FLT_MAX_EXP__ << '\n';
}

void print_vector(const std::vector<float> *vector)
{
    PROFILE_FUNCTION();
    const size_t vector_size = vector->size();

    for (int i = 0; i < 2; ++i)
    {
        std::cerr << "(";
        std::cerr << std::setprecision(15) << vector->operator[](i) << ") ";
    }
    std::cerr << "..."
              << " ";
    for (unsigned int i = vector_size - 2; i < vector_size; ++i)
    {
        std::cerr << "(";
        std::cerr << std::setprecision(15) << vector->operator[](i) << ") ";
    }
    std::cerr << "\n";
}

void plot_module(const std::vector<float> *module, const char *name = "./test.png")
{
    PROFILE_FUNCTION();
    namespace plt = matplotlibcpp;
    std::vector<double> t(module->size());

    // Set the size of output image to 1200x780 pixels
    plt::figure_size(1200, 780);

    // Plot line from given x and y data. Color is selected automatically.
    plt::plot(*module);

    // Add graph title
    plt::title("Sample figure");

    // Enable legend.
    plt::legend();

    // Save the image (file format is determined by the extension)
    plt::save(name);
    plt::show();
}

void plot_fft(const std::vector<float> *fft_freq, const std::vector<float> *fft, const char *name = "./test.png")
{
    PROFILE_FUNCTION();
    namespace plt = matplotlibcpp;

    // Set the size of output image to 1200x780 pixels
    plt::figure_size(1200, 780);

    // Plot line from given x and y data. Color is selected automatically.
    plt::plot(*fft_freq, *fft);

    // Add graph title
    // plt::title("DFT");

    plt::ylabel("V");
    plt::xlabel("Hz");

    plt::xlim(-200, 200);

    // Enable legend.
    plt::legend();

    // Save the image (file format is determined by the extension)
    plt::save(name);
    plt::show();
}

void print_telemetry(RPAS_telemetry *telemetry)
{
    PROFILE_FUNCTION();
    std::cerr << "Telemetry:\n"
              << "\tLat: " << telemetry->latitude_deg << '\n'
              << "\tLon: " << telemetry->longuitude_deg << '\n'
              << "\tHeight: " << telemetry->altitude_m << '\n'
              << "\tN_Sat: " << telemetry->n_sat << '\n'
              << "\tTS: " << telemetry->timestamp_ms << '\n';
}

void print_telemetry(RPAS_telemetry *telemetry, int n_proc)
{
    PROFILE_FUNCTION();
    std::cerr << "Telemetry " << n_proc << ":\n"
              << "\tLat: " << telemetry->latitude_deg << '\n'
              << "\tLon: " << telemetry->longuitude_deg << '\n'
              << "\tHeight: " << telemetry->altitude_m << '\n'
              << "\tN_Sat: " << telemetry->n_sat << '\n'
              << "\tTS: " << telemetry->timestamp_ms << '\n';
}