#include "do_work.h"

#define MODE 0

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
	std::ios_base::sync_with_stdio(false); // https://stackoverflow.com/questions/9371238/why-is-reading-lines-from-stdin-much-slower-in-c-than-python?rq=1
	Instrumentor::Get().BeginSession("Profile");

	if (MODE == 0)
	{
		// FROM CF32 FILE
		// DDM = 0.2
		// do_fromCF32("../cf32_files/fc_110300000_sr 500000_bw_600000_gain_True_1550863640.44.cf32", 500000, 1);

		// DDM = 0
		// do_fromCF32("../cf32_files/DDM0v2_250000s_110300000Hz_45g.cf32", 250000, 1); // ok
		// do_fromCF32("../cf32_files/DDM0v2_500000s_110300000Hz_45g.cf32", 500000, 1); // ok
		// do_fromCF32("../cf32_files/DDM0v2_2048000s_110300000Hz_45g.cf32", 2048000, 1); // ok
		// do_fromCF32("../cf32_files/DDM0v2_1000000s_110300000Hz_45g.cf32", 1000000, 1); // no muy bien
		// do_fromCF32("../cf32_files/DDM0v2_250000s_110300000Hz_45g.cf32", 250000, 1); // ok
		// do_fromCF32("../cf32_files/DDM0_2048000s_110300000Hz_0g.cf32", 2048000, 1); // ta maaaalll

		// DDM = -0.2
		do_fromCF32("../cf32_files/DDM-200_250000s_110300000Hz_45g.cf32", 250000, 1); // ok
		// do_fromCF32("../cf32_files/DDM-200v4_2048000s_110300000Hz_5g.cf32", 2048000, 1); // ta maaaalll
		// do_fromCF32("../cf32_files/DDM-200v3_2048000s_110300000Hz_15g.cf32", 2048000, 1); // ta maaaalll
		// do_fromCF32("../cf32_files/DDM-200v2_2048000s_110300000Hz_35g.cf32", 2048000, 1); // ta maaaalll
		// do_fromCF32("../cf32_files/DDM-200_500000s_110300000Hz_45g.cf32", 500000, 1); // ok
		// do_fromCF32("../cf32_files/DDM-200_2048000s_110300000Hz_45g.cf32", 2048000, 1); // ok
		// do_fromCF32("../cf32_files/DDM-200_1000000s_110300000Hz_45g.cf32", 1000000, 1); // ok pero DDM = -0.3

		Instrumentor::Get().EndSession();
		return 0;
	}

	else if (MODE == 1)
	{
		// FROM SDR
		int EXIT_STATUS = do_fromSDR();
		Instrumentor::Get().EndSession();
		return EXIT_STATUS;
	}

	else if (MODE == 2)
	{
		// ARRAY TEST
		do_ArryTest();
		Instrumentor::Get().EndSession();
		return 0;
	}

	else if (MODE == 3)
	{
		// CTRL-C TEST
		do_ctrlC_test();
		Instrumentor::Get().EndSession();
		return 0;
	}

	else if (MODE == 4)
	{
		// PIXHAWK TEST
		// int *argv_int[];
		do_pixhawk_test(argc, argv);
		Instrumentor::Get().EndSession();
		return 0;
	}

	else if (MODE == 5)
	{
		int a = main_mosquitto(argc, argv);
		return a;
	}

	else if (MODE == 6)
	{
		int a = test_sdr(argc, argv);
		return a;
	}
}

// std::ios_base::sync_with_stdio(false); // https://stackoverflow.com/questions/9371238/why-is-reading-lines-from-stdin-much-slower-in-c-than-python?rq=1
