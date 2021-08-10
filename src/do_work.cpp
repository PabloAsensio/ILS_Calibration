#include "do_work.h"

volatile sig_atomic_t STOP;
void inthand(int signum) { STOP = 1; }

static std::mutex s_Telemetry; // TO NOT MODIFY TELEMETRY WHILE COPYING

void store_signal(const Complex *signal, int n, std::string &signal_name)
{
    PROFILE_FUNCTION();
    std::cerr << "Storing into: " << signal_name << '\n';
    std::ofstream file(signal_name, std::ios::binary | std::ios::out);
    for (auto i = 0; i < n; ++i)
    {
        float real = signal[i].real();
        float imag = signal[i].imag();

        file.write(reinterpret_cast<const char *>(&real), sizeof(real));
        file.write(reinterpret_cast<const char *>(&imag), sizeof(imag));
    }
    file.close();
    std::cerr << "Stored: " << n << "\n";
}

std::string write_json(const RPAS_telemetry &telemetry, const ILS_data &ils_data, const std::string &json_name, int n_proc)
{
    PROFILE_FUNCTION();
    std::cerr << "Writing json into: " << json_name << '\n';
    std::ofstream out_json(json_name);
    out_json << "{\n"
             << "\t\"geometry\": {\n"
             << "\t\t\"type\": \"Point\",\n"
             << "\t\t\"coordinates\":  [\n"
             << "\t\t\t" << std::to_string(telemetry.longuitude_deg) << ",\n"
             << "\t\t\t" << std::to_string(telemetry.latitude_deg) << ",\n"
             << "\t\t\t" << std::to_string(telemetry.altitude_m) << "\n"
             << "\t\t]\n"
             << "\t},\n"
             << "\t\"type\": \"Feature\",\n"
             << "\t\"properties\": {\n"
             << "\t\t\"SATELLITES\": " << std::to_string(telemetry.n_sat) << ",\n"
             << "\t\t\"FIX_TYPE\": " << std::to_string(telemetry.fix_type) << ",\n"
             << "\t\t\"TIMESTAMP\": " << std::to_string(telemetry.timestamp_ms) << ",\n"

             << "\t\t\"DDM\": " << std::to_string(ils_data.DDM) << ",\n"
             << "\t\t\"SDM\": " << std::to_string(ils_data.SDM) << ",\n"
             << "\t\t\"ILS_HORIZONTAL_ANGLE\": " << std::to_string(ils_data.ILS_horizontal_angle) << ",\n"
             << "\t\t\"CALCULATED_HORIZONTAL_ANGLE\": " << std::to_string(ils_data.calculated_horizontal_angle) << ",\n"
             << "\t\t\"ILS_VERTICAL_ANGLE\": " << std::to_string(ils_data.ILS_vertical_angle) << ",\n"
             << "\t\t\"CALCULATED_VERTICAL_ANGLE\": " << std::to_string(ils_data.calculated_vertical_angle) << ",\n"
             << "\t\t\"COUSE\": " << std::to_string(ils_data.couse) << ",\n"
             << "\t\t\"CLEARENCE\": " << std::to_string(ils_data.clearence) << ",\n"
             << "\t\t\"N_PROCESS\": " << std::to_string(n_proc) << "\n"
             << "\t}\n"
             << "}";
    out_json.close();
    std::cerr << "JSON writed: " << n_proc << "\n";

    std::string msg_json = (std::string) "{\n" + (std::string) "\t\"geometry\": {\n" + (std::string) "\t\t\"type\": \"Point\",\n" + (std::string) "\t\t\"coordinates\":  [\n" + (std::string) "\t\t\t" + std::to_string(telemetry.longuitude_deg) + ",\n" + (std::string) "\t\t\t" + (std::string)std::to_string(telemetry.latitude_deg) + (std::string) ",\n" + (std::string) "\t\t\t" + (std::string)std::to_string(telemetry.altitude_m) + (std::string) "\n" + (std::string) "\t\t]\n" + (std::string) "\t},\n" + (std::string) "\t\"type\": \"Feature\",\n" + (std::string) "\t\"properties\": {\n" + (std::string) "\t\t\"SATELLITES\": " + (std::string)std::to_string(telemetry.n_sat) + (std::string) ",\n" + (std::string) "\t\t\"FIX_TYPE\": " + (std::string)std::to_string(telemetry.fix_type) + (std::string) ",\n" + (std::string) "\t\t\"TIMESTAMP\": " + (std::string)std::to_string(telemetry.timestamp_ms) + (std::string) ",\n"

                           + (std::string) "\t\t\"DDM\": " + (std::string)std::to_string(ils_data.DDM) + (std::string) ",\n" + (std::string) "\t\t\"SDM\": " + (std::string)std::to_string(ils_data.SDM) + (std::string) ",\n" + (std::string) "\t\t\"ILS_HORIZONTAL_ANGLE\": " + (std::string)std::to_string(ils_data.ILS_horizontal_angle) + (std::string) ",\n" + (std::string) "\t\t\"CALCULATED_HORIZONTAL_ANGLE\": " + (std::string)std::to_string(ils_data.calculated_horizontal_angle) + (std::string) ",\n" + (std::string) "\t\t\"ILS_VERTICAL_ANGLE\": " + (std::string)std::to_string(ils_data.ILS_vertical_angle) + (std::string) ",\n" + (std::string) "\t\t\"CALCULATED_VERTICAL_ANGLE\": " + (std::string)std::to_string(ils_data.calculated_vertical_angle) + (std::string) ",\n" + (std::string) "\t\t\"COUSE\": " + (std::string)std::to_string(ils_data.couse) + (std::string) ",\n" + (std::string) "\t\t\"CLEARENCE\": " + (std::string)std::to_string(ils_data.clearence) + (std::string) ",\n" + (std::string) "\t\t\"N_PROCESS\": " + (std::string)std::to_string(n_proc) + (std::string) "\n" + (std::string) "\t}\n" + (std::string) "}";
    return msg_json;
}

static std::mutex s_Complex; // TO NOT MODIFY SIGNAL WHILE COPYING
void do_ILS_calibration(const Complex *signal, const size_t signal_size,
                        int ret, int n_proc,
                        RPAS_telemetry telemetry,
                        mqtt_client *publisher_client)
{
    PROFILE_FUNCTION();
    std::cerr << "\nINICIALIZING!... " << n_proc << "\n";

    Complex *aux_signal = new Complex[signal_size];

    // TO NOT MODIFY SIGNAL WHILE COPYING
    {
        std::lock_guard<std::mutex> lock(s_Complex);
        for (unsigned i = 0; i < signal_size; ++i)
            aux_signal[i] = signal[i];
    }

    std::cerr << "Copyed!... " << n_proc << "\n";
    auto folder = "cf32_records/";
    // _mkdir(folder);
    auto signal_name = /*folder +*/ std::to_string(telemetry.timestamp_ms) + "_" + std::to_string(n_proc);
    auto signal_cf32_name = signal_name + ".cf32";
    auto signal_geojson_name = signal_name + ".geojson";
    store_signal(aux_signal, signal_size, signal_cf32_name);

    std::cerr << "NAMEEEE: " << signal_cf32_name << '\n';

    // print_complex(aux_signal, signal_size);
    print_telemetry(&telemetry, n_proc);

    // end copying

    // save_signal(aux_signal, signal_size, str_time);
    auto ddm_sdm_cal = ils_analisys(aux_signal, signal_size, SDR_SAMPLE_RATE);
    std::cerr << "DDM: " << ddm_sdm_cal.course_DDM << "\n";
    ILS_data ils_data;
    ils_data.DDM = ddm_sdm_cal.course_DDM;
    float course_DDM_angle = calculate_compass_bearing(ILS_LAT, ILS_LON, THR_LAT, THR_LAT) -
                             calculate_compass_bearing(ILS_LAT, ILS_LON, THR_LAT, THR_LAT);
    auto msg = write_json(telemetry, ils_data, signal_geojson_name, n_proc);

    // Sending info
    char *msg_pointer = new char[msg.length() + 1];
    std::strcpy(msg_pointer, msg.c_str());
    publisher_client->publish(NULL, "ENVIO_DE_DRON", strlen(msg_pointer), msg_pointer, 2);
    // std::cerr << msg;
    std::cerr << "Done!... " << n_proc << "\n";

    delete[] aux_signal;
}

int do_fromSDR()
{
    PROFILE_FUNCTION();
    signal(SIGINT, inthand);
    // TELEMETRY LOOP
    RPAS_telemetry telemetry;                         // telemetry data
    std::vector<std::future<void>> telemetry_Futures; // telemetry futures vector

    // print_telemetry(&telemetry);
    telemetry_Futures.push_back(std::async(std::launch::async, get_telemetry, &telemetry));

    // MQTT CONECTIONS AND LISTENER LOOP
    class mqtt_client *reciver_client;
    class mqtt_client *publisher_client;
    int rc;

    char client_id[] = CLIENT_ID;
    char host[] = BROKER_ADDRESS;
    int port = MQTT_PORT;

    mosqpp::lib_init();

    reciver_client = new mqtt_client("reciver_client", host, port);
    publisher_client = new mqtt_client("publisher_client", host, port);

    const char *reciver_username = "receivedrone";
    const char *reciver_password = "Rec3iv3Dr0n3";
    reciver_client->username_pw_set(reciver_username, reciver_password);

    const char *publisher_username = "sendrone";
    const char *publisher_password = "S3nDr0n3";
    publisher_client->username_pw_set(publisher_username, publisher_password);

    int reciver_connected = reciver_client->connect_async(host, port, 60);
    int publisher_connected = publisher_client->connect(host, port, 60);

    if (reciver_connected == 0)
        std::cerr << "reciver_client"
                  << " connected to: " << host << ":" << port << '\n';
    else
    {
        std::cerr << "RECIVER IMPOSSIBLE TO CONNECT\n";
        exit(1);
    }

    if (publisher_connected == 0)
        std::cerr << "publisher_client"
                  << " connected to: " << host << ":" << port << '\n';

    else
    {
        std::cerr << "PUBLISHER IMPOSSIBLE TO CONNECT\n";
        exit(1);
    }

    auto msg_ok = "LISTO";
    publisher_client->publish(NULL, "EXAMPLE_TOPIC", strlen(msg_ok), msg_ok, 2);

    std::vector<std::future<void>> mqtt_Futures;
    mqtt_Futures.push_back(std::async(std::launch::async, listen_on_topic, reciver_client, "EXAMPLE_TOPIC"));

    // SDR CONECTION AND MAIN LOOP
    try
    {
        // 0. enumerate devices (list all devices' information)
        SoapySDR::KwargsList results = SoapySDR::Device::enumerate();
        SoapySDR::Kwargs::iterator it;

        if (results.size() == 0)
            throw MyException("SDR NO DETECTED");

        for (unsigned int i = 0; i < results.size(); ++i)
        {
            printf("Found device #%d: ", i);
            for (it = results[i].begin(); it != results[i].end(); ++it)
            {
                printf("%s = %s\n", it->first.c_str(), it->second.c_str());
            }
            printf("\n");
        }

        // 1. create device instance

        //	1.1 set arguments
        //		args can be user defined or from the enumeration result
        //		We use first results as args here:
        SoapySDR::Kwargs args = results[0];

        //	1.2 make device
        SoapySDR::Device *sdr = SoapySDR::Device::make(args);

        if (sdr == NULL)
        {
            fprintf(stderr, "SoapySDR::Device::make failed\n");
            return EXIT_FAILURE;
        }

        // 2. query device info
        std::vector<std::string> str_list; //string list

        //	2.1 antennas
        str_list = sdr->listAntennas(SOAPY_SDR_RX, 0);
        printf("Rx antennas: ");
        for (unsigned int i = 0; i < str_list.size(); ++i)
            printf("%s,", str_list[i].c_str());
        printf("\n");

        //	2.2 gains
        str_list = sdr->listGains(SOAPY_SDR_RX, 0);
        printf("Rx Gains: ");
        for (unsigned int i = 0; i < str_list.size(); ++i)
            printf("%s, ", str_list[i].c_str());
        printf("\n");

        //	2.3. ranges(frequency ranges)
        SoapySDR::RangeList ranges = sdr->getFrequencyRange(SOAPY_SDR_RX, 0);
        printf("Rx freq ranges: ");
        for (unsigned int i = 0; i < ranges.size(); ++i)
            printf("[%g Hz . %g Hz], ", ranges[i].minimum(), ranges[i].maximum());
        printf("\n");

        // 3. apply settings
        sdr->setSampleRate(SOAPY_SDR_RX, SDR_RX_CHAN, SDR_SAMPLE_RATE);
        sdr->setFrequency(SOAPY_SDR_RX, SDR_RX_CHAN, SDR_CENTER_FREQ_HZ);
        sdr->setBandwidth(SOAPY_SDR_RX, SDR_RX_CHAN, SDR_BANDWIDTH_HZ);
        if (SDR_GAIN_AUTO)
            sdr->setGainMode(SOAPY_SDR_RX, SDR_RX_CHAN, SDR_GAIN_AUTO);
        else
        {
            sdr->setGainMode(SOAPY_SDR_RX, SDR_RX_CHAN, false);
            sdr->setGain(SOAPY_SDR_RX, SDR_RX_CHAN, "IFGR", SDR_GAIN_IFGR);
            sdr->setGain(SOAPY_SDR_RX, SDR_RX_CHAN, "RFGR", SDR_GAIN_RFGR);
        }

        // 4. setup a stream (complex floats)
        SoapySDR::Stream *rx_stream = sdr->setupStream(SOAPY_SDR_RX, SOAPY_SDR_CF32);
        if (rx_stream == NULL)
        {
            fprintf(stderr, "Failed\n");
            SoapySDR::Device::unmake(sdr);
            return EXIT_FAILURE;
        }
        sdr->activateStream(rx_stream, 0, 0, 0);

        // Maximum transmission unit (MTU) in number of elements.
        int num_rx_samps = sdr->getStreamMTU(rx_stream);

        std::cerr << "num_rx_samps: " << num_rx_samps << "\n";

        // 5. create a re-usable buffer for rx samples
        int samps = SDR_BUFFER_SIZE;
        // int samps = num_rx_samps;
        int samps_ret;
        std::complex<float> *buff = new std::complex<float>[SDR_BUFFER_SIZE];

        // thead func returns
        std::vector<std::future<void>> ils_Futures;

        // 6. receive some samples
        int process_counter = 0;
        while (!STOP && !KILL_PROGRAM)
        {
            void *buffs[] = {buff};
            int flags;
            long long time_ns;
            int ret = sdr->readStream(rx_stream, buffs, samps, flags, time_ns, 1e5);
            samps_ret = ret;
            // printf("ret = %d, flags = %d, time_ns = %lld\n", ret, flags, time_ns);
            std::cerr << "ret = " << ret << ", flags = " << flags
                      << ", time_ns = " << time_ns << "\n";
            if (ret > 0 && !STOP_MQTT)
            {
                // print_telemetry(&telemetry);
                ils_Futures.push_back(std::async(std::launch::async, do_ILS_calibration,
                                                 buff, samps_ret, ret, process_counter,
                                                 telemetry, publisher_client));
                // print_complex(buff, samps_ret);
                process_counter += 1;
                // KILL_PROGRAM = true;
                // STOP_MQTT = true;
            }
        }
        delete[] buff;
        std::cerr << "Desconnected to: " << host << ":" << port << '\n';
        mosqpp::lib_cleanup();

        // 7. shutdown the stream
        sdr->deactivateStream(rx_stream, 0, 0); //STOP streaming
        sdr->closeStream(rx_stream);

        // 8. cleanup device handle
        SoapySDR::Device::unmake(sdr);
        std::cerr << "Done\n";
        std::cerr << "Total process: " << process_counter << "\n";
        std::cerr << "ils_Futures size: " << ils_Futures.size() << "\n";
        std::cerr << "ils_Futures sizeof: " << sizeof(ils_Futures) << "\n";
        return EXIT_SUCCESS;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
}

void do_fromCF32(const char *filename, float sr, const unsigned int sampling = 1)
{
    PROFILE_FUNCTION();

    float f;
    std::ifstream fin(filename, std::ios::binary);
    if (!fin)
    {
        std::cerr << " Error, Couldn't find the file"
                  << "\n";
        exit(1);
    }

    // Find number of floats, so num_elements/2 = num_complex
    fin.seekg(0, std::ios::end);
    const size_t num_elements = fin.tellg() / sizeof(float);
    fin.seekg(0, std::ios::beg);

    // Checking lenght is even
    if (num_elements % 2 != 0)
    {
        std::cerr << " Error, size is odd."
                  << "\n";
        exit(1);
    }

    // Reserving array memory
    //number of samps
    const size_t buf_size = num_elements / 2;
    // std::cerr << buf_size << '\n';
    unsigned int samps = buf_size / sampling;
    std::complex<float> *signal = new std::complex<float>[samps];
    // std::complex<float> signal[samps];

    // Storing data in array
    unsigned int count = 0;
    unsigned int demod_count = 0;
    std::vector<float> complex_pair;
    for (unsigned int i = 0; i < num_elements; i++)
    {
        fin.read(reinterpret_cast<char *>(&f), sizeof(float));
        if (count < samps)
        {
            if (demod_count == sampling - 1 || demod_count == sampling)
            {
                // If complex pair done
                complex_pair.push_back(f);
                if (complex_pair.size() == 2)
                {
                    // Making complex pair

                    Complex aux_complex(complex_pair[0], complex_pair[1]);
                    signal[count] = aux_complex;
                    complex_pair.clear();
                    count++;
                    if (count > samps)
                    {
                        std::cerr << "Something was bad while charching data..." << '\n';
                        exit(1);
                    }
                }
            }
        }
        demod_count++;
        if (demod_count > sampling)
            demod_count = 0;
    }

    std::cerr << count << ' ' << ARRAY_SIZE(signal) << '\n';
    print_complex(signal, samps);

    auto ddm_sdm_cal = ils_analisys(signal, samps, sr);
    delete[] signal;

    std::cerr << "DDM: " << ddm_sdm_cal.course_DDM << '\n';
}

void test_array1(std::array<Complex, 20> a)
{
    std::cerr << "\ntest_array1 " << &a << '\n';
    for (unsigned int i = 0; i < a.size(); ++i)
    {
        std::cerr << i << ' ' << a.size() << ' ' << a[i] << '\n';
    }
}

void test_array2(Complex a[], const size_t n)
{
    std::cerr << "\ntest_array2 " << a << '\n';
    for (unsigned int i = 0; i < n; ++i)
    {
        std::cerr << i << ' ' << n << ' ' << a[i] << '\n';
    }
}

void do_ArryTest()
{
    PROFILE_FUNCTION();
    const int n = 20;
    std::complex<float> *array1 = new std::complex<float>[n];
    for (unsigned int i = 0; i < n; ++i)
    {
        std::cerr << "ARRAY1 " << array1[i] << '\n';
        array1[i] = std::complex<float>(i + 1, i + 1);
        // array1[i].imag() = i + 1;
    }
    std::cerr << "\nARRAY1 " << array1 << '\n';
    test_array2(array1, n);

    delete[] array1;
}

void do_ctrlC_test()
{
    signal(SIGINT, inthand);

    int counter = 0;
    while (!STOP)
    {
        // std::cerr << "Counter: " << counter << '\n';
        counter += 1;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cerr << "\nCounter: " << counter << '\n';
        // if (counter > 3)
        //     STOP = 1;
    }
    std::cerr << "\nExiting safely\n";

    system(0);
}

int do_pixhawk_test(int argc, char *argv[])
{
    // int _exit = main_pixhawk(argc, argv);
    int _exit = test_telemetry(argc, argv);
    return _exit;
}

int test_sdr(int argc, char *argv[])
{
    // 0. enumerate devices (list all devices' information)
    SoapySDR::KwargsList results = SoapySDR::Device::enumerate();
    SoapySDR::Kwargs::iterator it;

    for (int i = 0; i < results.size(); ++i)
    {
        printf("Found device #%d: ", i);
        for (it = results[i].begin(); it != results[i].end(); ++it)
        {
            printf("%s = %s\n", it->first.c_str(), it->second.c_str());
        }
        printf("\n");
    }

    // 1. create device instance

    //	1.1 set arguments
    //		args can be user defined or from the enumeration result
    //		We use first results as args here:
    SoapySDR::Kwargs args = results[0];

    //	1.2 make device
    SoapySDR::Device *sdr = SoapySDR::Device::make(args);

    if (sdr == NULL)
    {
        fprintf(stderr, "SoapySDR::Device::make failed\n");
        return EXIT_FAILURE;
    }

    // 2. query device info
    std::vector<std::string> str_list; //string list

    //	2.1 antennas
    str_list = sdr->listAntennas(SOAPY_SDR_RX, 0);
    printf("Rx antennas: ");
    for (int i = 0; i < str_list.size(); ++i)
        printf("%s,", str_list[i].c_str());
    printf("\n");

    //	2.2 gains
    str_list = sdr->listGains(SOAPY_SDR_RX, 0);
    printf("Rx Gains: ");
    for (int i = 0; i < str_list.size(); ++i)
        printf("%s, ", str_list[i].c_str());
    printf("\n");

    //	2.3. ranges(frequency ranges)
    SoapySDR::RangeList ranges = sdr->getFrequencyRange(SOAPY_SDR_RX, 0);
    printf("Rx freq ranges: ");
    for (int i = 0; i < ranges.size(); ++i)
        printf("[%g Hz -> %g Hz], ", ranges[i].minimum(), ranges[i].maximum());
    printf("\n");

    // 3. apply settings
    sdr->setSampleRate(SOAPY_SDR_RX, 0, 10e6);

    sdr->setFrequency(SOAPY_SDR_RX, 0, 433e6);

    // 4. setup a stream (complex floats)
    SoapySDR::Stream *rx_stream = sdr->setupStream(SOAPY_SDR_RX, SOAPY_SDR_CF32);
    if (rx_stream == NULL)
    {
        fprintf(stderr, "Failed\n");
        SoapySDR::Device::unmake(sdr);
        return EXIT_FAILURE;
    }
    sdr->activateStream(rx_stream, 0, 0, 0);

    // 5. create a re-usable buffer for rx samples
    std::complex<float> buff[1024];

    // 6. receive some samples
    for (int i = 0; i < 10; ++i)
    {
        void *buffs[] = {buff};
        int flags;
        long long time_ns;
        int ret = sdr->readStream(rx_stream, buffs, 1024, flags, time_ns, 1e5);
        printf("ret = %d, flags = %d, time_ns = %lld\n", ret, flags, time_ns);
    }

    // 7. shutdown the stream
    sdr->deactivateStream(rx_stream, 0, 0); //stop streaming
    sdr->closeStream(rx_stream);

    // 8. cleanup device handle
    SoapySDR::Device::unmake(sdr);
    printf("Done\n");

    return EXIT_SUCCESS;
}