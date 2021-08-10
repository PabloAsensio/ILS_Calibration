#include <ils_analisys.h>

DDM_SDM ils_analisys(const std::complex<float> *signal, unsigned int ret, float samps_hz)
{
    PROFILE_FUNCTION();

    // Reserving memery for input data to fft
    fftwf_complex *inModule = new fftwf_complex[ret];

    // Asigning input data to fft format for demod signal
    unsigned int counter = 0;
    for (unsigned int i = 0; i < ret; ++i)
    {
        // Demod signal
        counter++;
        inModule[i][REAL] = std::abs(signal[i]);
        inModule[i][IMAG] = 0;
    }

    // Check lenghts of signals
    if (counter != ret)
    {
        std::cerr << "WARNING! DIFFERENT LENGHTS!\n";
        // exit(1);
    }

    // Reserving memory to fft and doing FFT
    fftwf_complex *outfft_pos = new fftwf_complex[ret];
    fft::fft(inModule, ret, outfft_pos);
    delete[] inModule; // No more used

    // Reserving memory to fft module and doing FFT
    std::vector<float> *fft_pos_module = new std::vector<float>;
    fft_pos_module->reserve(ret);
    compute_fft_module(outfft_pos, ret, fft_pos_module);
    delete[] outfft_pos; // No more used

    std::vector<float> *fft_freq = new std::vector<float>;
    compute_freq_axis(fft_freq, ret, 1.0f / samps_hz);


    DDM_SDM output_data;

    float course_carrier = max_tone_base_band_rectangle_filter(fft_pos_module, fft_freq, 0, 10) / 2;
    float course_tone90 = max_tone_base_band_rectangle_filter(fft_pos_module, fft_freq, 90, 10);
    float course_tone150 = max_tone_base_band_rectangle_filter(fft_pos_module, fft_freq, 150, 10);
    output_data.course_DDM = 0.4 * (course_tone90 - course_tone150) / (course_tone90 + course_tone150);
    output_data.course_SDM = (course_tone90 + course_tone150) / course_carrier;
    output_data.course_modulation_percentage_90 = 100 * course_tone90 / course_carrier;
    output_data.course_modulation_percentage_150 = 100 * course_tone150 / course_carrier;
    output_data.course_index_modulation_90 = 0.4 * course_tone90 / (course_tone90 + course_tone150);
    output_data.course_index_modulation_150 = 0.4 * course_tone150 / (course_tone90 + course_tone150);

    float clearance_carrier_freq = freq_max_tone(fft_pos_module, fft_freq, 8000, 1000) / 2;
    float clearance_tone90 = max_tone_base_band_rectangle_filter(fft_pos_module, fft_freq, clearance_carrier_freq + 90, 10);
    float clearance_tone150 = max_tone_base_band_rectangle_filter(fft_pos_module, fft_freq, clearance_carrier_freq + 150, 10);
    output_data.clearance_DDM = 0.4 * (clearance_tone90 - clearance_tone150) / (clearance_tone90 + clearance_tone150);
    output_data.clearance_SDM = (clearance_tone90 + clearance_tone150) / clearance_carrier_freq;
    output_data.clearance_modulation_percentage_90 = 100 * clearance_tone90 / clearance_carrier_freq;
    output_data.clearance_modulation_percentage_150 = 100 * clearance_tone150 / clearance_carrier_freq;
    output_data.clearance_index_modulation_90 = 0.4 * clearance_tone90 / (clearance_tone90 + clearance_tone150);
    output_data.clearance_index_modulation_150 = 0.4 * clearance_tone150 / (clearance_tone90 + clearance_tone150);

    std::cerr << "course_coarrirer: " << course_carrier << '\n';
    std::cerr << "course_tone90: " << course_tone90 << '\n';
    std::cerr << "course_tone150: " << course_tone150 << '\n';
    std::cerr << "course_DDM: " << output_data.course_DDM << '\n';
    std::cerr << "course_SDM: " << output_data.course_SDM << '\n';
    std::cerr << "course_modulation_percentage_90: " << output_data.course_modulation_percentage_90 << '\n';
    std::cerr << "course_modulation_percentage_150: " << output_data.course_modulation_percentage_150 << '\n';
    std::cerr << "sum modulation percentages: " << output_data.course_modulation_percentage_150 + output_data.course_modulation_percentage_90 << '\n';
    std::cerr << "course_index_modulation_90: " << output_data.course_index_modulation_90 << '\n';
    std::cerr << "course_index_modulation_150: " << output_data.course_index_modulation_150 << '\n';

    plot_fft(fft_freq, fft_pos_module, "fft.png");

    delete fft_pos_module;
    delete fft_freq;

    return output_data;
}