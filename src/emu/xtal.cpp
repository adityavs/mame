// license:BSD-3-Clause
// copyright-holders:Nicola Salmoria
/*************************************************************************

    xtal.h

    Documentation and consistent naming for known existing crystals.

***************************************************************************

    PCB Crystal Reference
    ----------------------
       _____     ________
       |16 |    |o       |
       |MHZ|    |16.0MHZ |
       |___|    |________|
       |   |

    There is a finite list of _manufactured_ crystals. This file aims
    to document all of the known speeds that crystals were available in.
    Feel free to add to the list if you find another speed crystal on
    a PCB, but please DON'T GUESS!

    Remember that some very high frequencies (typically above 100MHz) are
    generated by a frequency multiplying IC from a lower frequency
    crystal.

    This is intended as a reference of existing parts to prevent incorrect
    clock speed measurements with digital frequency counters being used
    in drivers. When measuring clocks, remember that most parts have a
    tolerance so be sure to reference existing parts only and not just
    accept direct readings as 100% true.

    MAME supports fractions in crystal frequencies, but for historical
    and readability reasons we tend to write the closest integer
    value. For example, 3.579545_MHz_XTAL should actually be
    3.579545454545...MHz (315/88). This is no problem though: see above
    note about tolerance.

    In the "Examples" column, please don't add 1000 examples, this is just
    for interest, so two or three examples is enough.
    The actual reference where the xtals are used can be found in the
    driver files by searching for the frequency (e.g. "XTAL(4'915'200)")

    (Thanks to Guru for starting this documentation.)

**************************************************************************/

#include "emu.h"

#include <cfloat>
#include <cmath>


// This array *must* stay in order, it's binary-searched
const double XTAL::known_xtals[] = {
/*
    Frequency       Sugarvassed            Examples
    -----------  ----------------------    ---------------------------------------- */
		 32'768, /* 32.768_kHz_XTAL        Used to drive RTC chips */
		 38'400, /* 38.4_kHz_XTAL          Resonator */
		384'000, /* 384_kHz_XTAL           Resonator - Commonly used for driving OKI MSM5205 */
		400'000, /* 400_kHz_XTAL           Resonator - OKI MSM5205 on Great Swordman h/w */
		430'000, /* 430_kHz_XTAL           Resonator */
		455'000, /* 455_kHz_XTAL           Resonator - OKI MSM5205 on Gladiator h/w */
		512'000, /* 512_kHz_XTAL           Resonator - Toshiba TC8830F */
		600'000, /* 600_kHz_XTAL           - */
		640'000, /* 640_kHz_XTAL           Resonator - NEC UPD7759, Texas Instruments Speech Chips @ 8khz */
		960'000, /* 960_kHz_XTAL           Resonator - Xerox Notetaker Keyboard UART */
	  1'000'000, /* 1_MHz_XTAL             Used to drive OKI M6295 chips */
	  1'008'000, /* 1.008_MHz_XTAL         Acorn Microcomputer (System 1) */
	  1'056'000, /* 1.056_MHz_XTAL         Resonator - OKI M6295 on Trio The Punch h/w */
	  1'294'400, /* 1.2944_MHz_XTAL        BBN BitGraph PSG */
	  1'689'600, /* 1.6896_MHz_XTAL        Diablo 1355WP Printer */
	  1'750'000, /* 1.75_MHz_XTAL          RCA CDP1861 */
	  1'797'100, /* 1.7971_MHz_XTAL        SWTPC 6800 (with MIKBUG) */
	  1'843'200, /* 1.8432_MHz_XTAL        Bondwell 12/14 */
	  2'000'000, /* 2_MHz_XTAL             - */
	  2'012'160, /* 2.01216_MHz_XTAL       Cidelsa Draco sound board */
	  2'097'152, /* 2.097152_MHz_XTAL      Icatel 1995 - Brazilian public payphone */
	  2'457'600, /* 2.4576_MHz_XTAL        Atari ST MFP */
	  2'500'000, /* 2.5_MHz_XTAL           Janken Man units */
	  2'950'000, /* 2.95_MHz_XTAL          Playmatic MPU-C, MPU-III & Sound-3 */
	  3'000'000, /* 3_MHz_XTAL             Probably only used to drive 68705 or similar MCUs on 80's Taito PCBs */
	  3'072'000, /* 3.072_MHz_XTAL         INS 8520 input clock rate */
	  3'120'000, /* 3.12_MHz_XTAL          SP0250 clock on Gottlieb games */
	  3'521'280, /* 3.52128_MHz_XTAL       RCA COSMAC VIP */
	  3'570'000, /* 3.57_MHz_XTAL          Telmac TMC-600 */
	  3'578'640, /* 3.57864_MHz_XTAL       Atari Portfolio PCD3311T */
	  3'579'545, /* 3.579545_MHz_XTAL      NTSC color subcarrier, extremely common, used on 100's of PCBs (Keytronic custom part #48-300-010 is equivalent) */
	  3'686'400, /* 3.6864_MHz_XTAL        Baud rate clock for MC68681 and similar UARTs */
	  3'840'000, /* 3.84_MHz_XTAL          Fairlight CMI Alphanumeric Keyboard */
	  3'900'000, /* 3.9_MHz_XTAL           Resonator - Used on some Fidelity boards */
	  4'000'000, /* 4_MHz_XTAL             - */
	  4'028'000, /* 4.028_MHz_XTAL         Sony SMC-777 */
	  4'032'000, /* 4.032_MHz_XTAL         GRiD Compass modem board */
	  4'096'000, /* 4.096_MHz_XTAL         Used to drive OKI M9810 chips */
	  4'194'304, /* 4.194304_MHz_XTAL      Used to drive MC146818 / Nintendo Game Boy */
	  4'224'000, /* 4.224_MHz_XTAL         Used to drive OKI M6295 chips, usually with /4 divider */
	  4'410'000, /* 4.41_MHz_XTAL          Pioneer PR-8210 ldplayer */
	  4'433'610, /* 4.43361_MHz_XTAL       Cidelsa Draco */
	  4'433'619, /* 4.433619_MHz_XTAL      PAL color subcarrier (technically 4.43361875mhz)*/
	  4'608'000, /* 4.608_MHz_XTAL         Luxor ABC-77 keyboard (Keytronic custom part #48-300-107 is equivalent) */
	  4'915'200, /* 4.9152_MHz_XTAL        - */
	  5'000'000, /* 5_MHz_XTAL             Mutant Night */
	  5'068'800, /* 5.0688_MHz_XTAL        Usually used as MC2661 or COM8116 baud rate clock */
	  5'185'000, /* 5.185_MHz_XTAL         Intel INTELLEC® 4 */
	  5'460'000, /* 5.46_MHz_XTAL          ec1840 and ec1841 keyboard */
	  5'529'600, /* 5.5296_MHz_XTAL        Kontron PSI98 keyboard */
	  5'626'000, /* 5.626_MHz_XTAL         RCA CDP1869 PAL dot clock */
	  5'670'000, /* 5.67_MHz_XTAL          RCA CDP1869 NTSC dot clock */
	  5'714'300, /* 5.7143_MHz_XTAL        Cidelsa Destroyer, TeleVideo serial keyboards */
	  5'911'000, /* 5.911_MHz_XTAL         Philips Videopac Plus G7400 */
	  5'990'400, /* 5.9904_MHz_XTAL        Luxor ABC 800 keyboard (Keytronic custom part #48-300-008 is equivalent) */
	  6'000'000, /* 6_MHz_XTAL             American Poker II, Taito SJ System */
	  6'144'000, /* 6.144_MHz_XTAL         Used on Alpha Denshi early 80's games sound board, Casio FP-200 and Namco Universal System 16 */
	  6'400'000, /* 6.4_MHz_XTAL           Textel Compact */
	  6'500'000, /* 6.5_MHz_XTAL           Jupiter Ace */
	  6'880'000, /* 6.88_MHz_XTAL          Barcrest MPU4 */
	  6'900'000, /* 6.9_MHz_XTAL           BBN BitGraph CPU */
	  7'000'000, /* 7_MHz_XTAL             Jaleco Mega System PCBs */
	  7'159'090, /* 7.15909_MHz_XTAL       Blood Bros (2x NTSC subcarrier) */
	  7'372'800, /* 7.3728_MHz_XTAL        - */
	  7'864'300, /* 7.8643_MHz_XTAL        Used on InterFlip games as video clock */
	  7'987'000, /* 7.987_MHz_XTAL         PC9801-86 YM2608 clock */
	  8'000'000, /* 8_MHz_XTAL             Extremely common, used on 100's of PCBs */
	  8'200'000, /* 8.2_MHz_XTAL           Universal Mr. Do - Model 8021 PCB */
	  8'388'000, /* 8.388_MHz_XTAL         Nintendo Game Boy Color */
	  8'448'000, /* 8.448_MHz_XTAL         Banpresto's Note Chance - Used to drive OKI M6295 chips, usually with /8 divider */
	  8'467'200, /* 8.4672_MHz_XTAL        Subsino's Ying Hua Lian */
	  8'664'000, /* 8.664_MHz_XTAL         Touchmaster */
	  8'700'000, /* 8.7_MHz_XTAL           Tandberg TDV 2324 */
	  8'867'236, /* 8.867236_MHz_XTAL      RCA CDP1869 PAL color clock (~2x PAL subcarrier) */
	  8'867'238, /* 8.867238_MHz_XTAL      ETI-660 (~2x PAL subcarrier) */
	  8'945'000, /* 8.945_MHz_XTAL         Hit Me */
	  9'216'000, /* 9.216_MHz_XTAL         Conitec PROF-180X */
	  9'828'000, /* 9.828_MHz_XTAL         Universal PCBs */
	  9'830'400, /* 9.8304_MHz_XTAL        Epson PX-8 */
	  9'832'000, /* 9.832_MHz_XTAL         Robotron A7150 */
	  9'877'680, /* 9.87768_MHz_XTAL       Microterm 420 */
	  9'987'000, /* 9.987_MHz_XTAL         Crazy Balloon */
	 10'000'000, /* 10_MHz_XTAL            - */
	 10'137'600, /* 10.1376_MHz_XTAL       Wyse WY-100 */
	 10'245'000, /* 10.245_MHz_XTAL        PES Speech box */
	 10'380'000, /* 10.38_MHz_XTAL         Fairlight Q219 Lightpen/Graphics Card */
	 10'500'000, /* 10.5_MHz_XTAL          Agat-7 */
	 10'595'000, /* 10.595_MHz_XTAL        Mad Alien */
	 10'644'500, /* 10.6445_MHz_XTAL       TRS-80 Model I */
	 10'687'500, /* 10.6875_MHz_XTAL       BBC Bridge Companion */
	 10'694'250, /* 10.69425_MHz_XTAL      Xerox 820 */
	 10'717'200, /* 10.7172_MHz_XTAL       Eltec EurocomII */
	 10'730'000, /* 10.73_MHz_XTAL         Ruleta RE-900 VDP Clock */
	 10'733'000, /* 10.733_MHz_XTAL        The Fairyland Story */
	 10'738'635, /* 10.738635_MHz_XTAL     TMS9918 family (3x NTSC subcarrier) */
	 10'816'000, /* 10.816_MHz_XTAL        Universal 1979-1980 (Cosmic Alien, etc) */
	 10'920'000, /* 10.92_MHz_XTAL         ADDS Viewpoint 60, Viewpoint A2 */
	 11'000'000, /* 11_MHz_XTAL            Mario I8039 sound */
	 11'059'200, /* 11.0592_MHz_XTAL       Used with MCS-51 to generate common baud rates */
	 11'200'000, /* 11.2_MHz_XTAL          New York, New York */
	 11'289'000, /* 11.289_MHz_XTAL        Vanguard */
	 11'400'000, /* 11.4_MHz_XTAL          HP 9845 */
	 11'668'800, /* 11.6688_MHz_XTAL       Gameplan pixel clock */
	 11'800'000, /* 11.8_MHz_XTAL          IBM PC Music Feature Card */
	 11'980'800, /* 11.9808_MHz_XTAL       Luxor ABC 80 */
	 12'000'000, /* 12_MHz_XTAL            Extremely common, used on 100's of PCBs */
	 12'057'600, /* 12.0576_MHz_XTAL       Poly 1 (38400 * 314) */
	 12'096'000, /* 12.096_MHz_XTAL        Some early 80's Atari games */
	 12'288'000, /* 12.288_MHz_XTAL        Sega Model 3 digital audio board */
	 12'324'000, /* 12.324_MHz_XTAL        Otrona Attache */
	 12'432'000, /* 12.432_MHz_XTAL        Kaneko Fly Boy/Fast Freddie Hardware */
	 12'472'500, /* 12.4725_MHz_XTAL       Bonanza's Mini Boy 7 */
	 12'480'000, /* 12.48_MHz_XTAL         TRS-80 Model II */
	 12'500'000, /* 12.5_MHz_XTAL          Red Alert audio board */
	 12'672'000, /* 12.672_MHz_XTAL        TRS-80 Model 4 80*24 video */
	 12'800'000, /* 12.8_MHz_XTAL          Cave CV1000 */
	 12'854'400, /* 12.8544_MHz_XTAL       Alphatronic P3 */
	 12'936'000, /* 12.936_MHz_XTAL        CDC 721 */
	 12'979'200, /* 12.9792_MHz_XTAL       Exidy 440 */
	 13'300'000, /* 13.3_MHz_XTAL          BMC bowling */
	 13'330'560, /* 13.33056_MHz_XTAL      Taito L */
	 13'333'000, /* 13.333_MHz_XTAL        Ojanko High School */
	 13'400'000, /* 13.4_MHz_XTAL          TNK3, Ikari Warriors h/w */
	 13'478'400, /* 13.4784_MHz_XTAL       TeleVideo 970 80-column display clock */
	 13'495'200, /* 13.4952_MHz_XTAL       Used on Shadow Force pcb and maybe other Technos pcbs? */
	 13'516'800, /* 13.5168_MHz_XTAL       Kontron KDT6 */
	 13'608'000, /* 13.608_MHz_XTAL        TeleVideo 910 & 925 */
	 13'824'000, /* 13.824_MHz_XTAL        Robotron PC-1715 display circuit */
	 14'000'000, /* 14_MHz_XTAL            - */
	 14'112'000, /* 14.112_MHz_XTAL        Timex/Sinclair TS2068 */
	 14'192'640, /* 14.19264_MHz_XTAL      Central Data 2650 */
	 14'218'000, /* 14.218_MHz_XTAL        Dragon */
	 14'300'000, /* 14.3_MHz_XTAL          Agat-7 */
	 14'314'000, /* 14.314_MHz_XTAL        Taito TTL Board  */
	 14'318'181, /* 14.318181_MHz_XTAL     Extremely common, used on 100's of PCBs (4x NTSC subcarrier) */
	 14'705'882, /* 14.705882_MHz_XTAL     Aleck64 */
	 14'745'600, /* 14.7456_MHz_XTAL       Namco System 12 & System Super 22/23 for JVS */
	 14'784'000, /* 14.784_MHz_XTAL        Zenith Z-29 */
	 14'916'000, /* 14.916_MHz_XTAL        ADDS Viewpoint 122 */
	 14'976'000, /* 14.976_MHz_XTAL        CIT-101 80-column display clock */
	 15'000'000, /* 15_MHz_XTAL            Sinclair QL, Amusco Poker */
	 15'148'800, /* 15.1488_MHz_XTAL       Zentec 9002/9003 */
	 15'288'000, /* 15.288_MHz_XTAL        DEC VT220 80-column display clock */
	 15'300'720, /* 15.30072_MHz_XTAL      Microterm 420 */
	 15'360'000, /* 15.36_MHz_XTAL         Visual 1050 */
	 15'400'000, /* 15.4_MHz_XTAL          DVK KSM */
	 15'468'480, /* 15.46848_MHz_XTAL      Bank Panic h/w, Sega G80 */
	 15'582'000, /* 15.582_MHz_XTAL        Zentec Zephyr */
	 15'700'000, /* 15.700_MHz_XTAL        Motogonki */
	 15'897'600, /* 15.8976_MHz_XTAL       IAI Swyft */
	 15'920'000, /* 15.92_MHz_XTAL         HP Integral PC */
	 15'974'400, /* 15.9744_MHz_XTAL       Osborne 1 (9600 * 52 * 32) */
	 16'000'000, /* 16_MHz_XTAL            Extremely common, used on 100's of PCBs */
	 16'097'280, /* 16.09728_MHz_XTAL      DEC VT240 (1024 * 262 * 60) */
	 16'128'000, /* 16.128_MHz_XTAL        Fujitsu FM-7 */
	 16'384'000, /* 16.384_MHz_XTAL        - */
	 16'400'000, /* 16.4_MHz_XTAL          MS 6102 */
	 16'572'000, /* 16.572_MHz_XTAL        Micro-Term ACT-5A */
	 16'588'800, /* 16.5888_MHz_XTAL       SM 7238 */
	 16'669'800, /* 16.6698_MHz_XTAL       Qume QVT-102 */
	 16'670'000, /* 16.67_MHz_XTAL         - */
	 16'777'216, /* 16.777216_MHz_XTAL     Nintendo Game Boy Advance */
	 16'934'400, /* 16.9344_MHz_XTAL       Usually used to drive 90's Yamaha OPL/FM chips (44100 * 384) */
	 17'064'000, /* 17.064_MHz_XTAL        Memorex 1377 */
	 17'360'000, /* 17.36_MHz_XTAL         OMTI Series 10 SCSI controller */
	 17'550'000, /* 17.55_MHz_XTAL         HP 264x display clock (50 Hz configuration) */
	 17'600'000, /* 17.6_MHz_XTAL          LSI Octopus */
	 17'734'470, /* 17.73447_MHz_XTAL      (~4x PAL subcarrier) */
	 17'734'472, /* 17.734472_MHz_XTAL     actually ~4x PAL subcarrier */
	 17'971'200, /* 17.9712_MHz_XTAL       Compucolor II, Hazeltine Esprit III */
	 18'000'000, /* 18_MHz_XTAL            S.A.R, Ikari Warriors 3 */
	 18'432'000, /* 18.432_MHz_XTAL        Extremely common, used on 100's of PCBs (48000 * 384) */
	 18'480'000, /* 18.48_MHz_XTAL         Wyse WY-100 video */
	 18'575'000, /* 18.575_MHz_XTAL        Visual 102, Visual 220 */
	 18'720'000, /* 18.72_MHz_XTAL         Nokia MikroMikko 1 */
	 18'869'600, /* 18.8696_MHz_XTAL       Memorex 2178 */
	 19'339'600, /* 19.3396_MHz_XTAL       TeleVideo TVI-955 80-column display clock */
	 19'600'000, /* 19.6_MHz_XTAL          Universal Mr. Do - Model 8021 PCB */
	 19'602'000, /* 19.602_MHz_XTAL        Ampex 210+ 80-column display clock */
	 19'660'800, /* 19.6608_MHz_XTAL       Euro League (bootleg), labeled as "UKI 19.6608 20PF" */
	 19'661'400, /* 19.6614_MHz_XTAL       Wyse WY-30 */
	 19'923'000, /* 19.923_MHz_XTAL        Cinematronics vectors */
	 19'968'000, /* 19.968_MHz_XTAL        Used mostly by some Taito games */
	 20'000'000, /* 20_MHz_XTAL            - */
	 20'160'000, /* 20.16_MHz_XTAL         Nintendo 8080 */
	 20'275'200, /* 20.2752_MHz_XTAL       TRS-80 Model III */
	 20'625'000, /* 20.625_MHz_XTAL        SM 7238 */
	 20'790'000, /* 20.79_MHz_XTAL         Blockade-hardware Gremlin games */
	 21'000'000, /* 21_MHz_XTAL            Lock-On pixel clock */
	 21'052'600, /* 21.0526_MHz_XTAL       NEC PC-98xx pixel clock */
	 21'060'000, /* 21.06_MHz_XTAL         HP 264x display clock (60 Hz configuration) */
	 21'254'400, /* 21.2544_MHz_XTAL       TeleVideo 970 132-column display clock */
	 21'281'370, /* 21.28137_MHz_XTAL      Radica Tetris (PAL) */
	 21'300'000, /* 21.3_MHz_XTAL          - */
	 21'477'272, /* 21.477272_MHz_XTAL     BMC bowling, some Data East 90's games, Vtech Socrates; (6x NTSC subcarrier) */
	 22'000'000, /* 22_MHz_XTAL            - */
	 22'032'000, /* 22.032_MHz_XTAL        Intellec Series II I/O controller */
	 22'096'000, /* 22.096_MHz_XTAL        ADDS Viewpoint 122 */
	 22'118'400, /* 22.1184_MHz_XTAL       Amusco Poker */
	 22'321'000, /* 22.321_MHz_XTAL        Apple LaserWriter II NT */
	 22'464'000, /* 22.464_MHz_XTAL        CIT-101 132-column display clock */
	 22'656'000, /* 22.656_MHz_XTAL        Super Pinball Action (~1440x NTSC line rate) */
	 22'896'000, /* 22.896_MHz_XTAL        DEC VT220 132-column display clock */
	 23'814'000, /* 23.814_MHz_XTAL        TeleVideo TVI-912, 920 & 950 */
	 23'961'600, /* 23.9616_MHz_XTAL       Osborne 4 (Vixen) */
	 24'000'000, /* 24_MHz_XTAL            Mario, 80's Data East games, 80's Konami games */
	 24'073'400, /* 24.0734_MHz_XTAL       DEC Rainbow 100 */
	 24'576'000, /* 24.576_MHz_XTAL        Pole Position h/w, Model 3 CPU board */
	 24'883'200, /* 24.8832_MHz_XTAL       DEC VT100 */
	 25'000'000, /* 25_MHz_XTAL            Namco System 22, Taito GNET, Dogyuun h/w */
	 25'174'800, /* 25.1748_MHz_XTAL       Sega System 16A/16B (1600x NTSC line rate) */
	 25'200'000, /* 25.2_MHz_XTAL          Tektronix 4404 video clock */
	 25'398'360, /* 25.39836_MHz_XTAL      Tandberg TDV 2324 */
	 25'400'000, /* 25.4_MHz_XTAL          PC9801-86 PCM base clock */
	 25'447'000, /* 25.447_MHz_XTAL        Namco EVA3A (Funcube2) */
	 25'590'906, /* 25.590906_MHz_XTAL     Atari Jaguar NTSC */
	 25'593'900, /* 25.5939_MHz_XTAL       Atari Jaguar PAL */
	 25'771'500, /* 25.7715_MHz_XTAL       HP-2622A */
	 25'920'000, /* 25.92_MHz_XTAL         ADDS Viewpoint 60 */
	 26'000'000, /* 26_MHz_XTAL            Gaelco PCBs */
	 26'366'000, /* 26.366_MHz_XTAL        DEC VT320 */
	 26'580'000, /* 26.58_MHz_XTAL         Wyse WY-60 80-column display clock */
	 26'601'712, /* 26.601712_MHz_XTAL     Astro Corp.'s Show Hand, PAL Vtech/Yeno Socrates (6x PAL subcarrier) */
	 26'666'000, /* 26.666_MHz_XTAL        Imagetek I4100/I4220/I4300 */
	 26'666'666, /* 26.666666_MHz_XTAL     Irem M92 but most use 27MHz */
	 26'686'000, /* 26.686_MHz_XTAL        Typically used on 90's Taito PCBs to drive the custom chips */
	 26'989'200, /* 26.9892_MHz_XTAL       TeleVideo 965 */
	 27'000'000, /* 27_MHz_XTAL            Some Banpresto games macrossp, Irem M92 and 90's Toaplan games */
	 27'164'000, /* 27.164_MHz_XTAL        Typically used on 90's Taito PCBs to drive the custom chips */
	 27'210'900, /* 27.2109_MHz_XTAL       LA Girl */
	 27'562'000, /* 27.562_MHz_XTAL        Visual 220 */
	 28'000'000, /* 28_MHz_XTAL            - */
	 28'322'000, /* 28.322_MHz_XTAL        Saitek RISC 2500, Mephisto Montreux */
	 28'375'160, /* 28.37516_MHz_XTAL      Amiga PAL systems */
	 28'475'000, /* 28.475_MHz_XTAL        CoCo 3 PAL */
	 28'480'000, /* 28.48_MHz_XTAL         Chromatics CGC-7900 */
	 28'636'363, /* 28.636363_MHz_XTAL     Later Leland games and Atari GT, Amiga NTSC, Raiden2 h/w (8x NTSC subcarrier)*/
	 28'640'000, /* 28.64_MHz_XTAL         Fukki FG-1c AI AM-2 PCB */
	 28'700'000, /* 28.7_MHz_XTAL          - */
	 29'376'000, /* 29.376_MHz_XTAL        Qume QVT-103 */
	 29'491'200, /* 29.4912_MHz_XTAL       Xerox Alto-II system clock (tagged 29.4MHz in the schematics) */
	 30'000'000, /* 30_MHz_XTAL            Impera Magic Card */
	 30'476'100, /* 30.4761_MHz_XTAL       Taito JC */
	 30'800'000, /* 30.8_MHz_XTAL          15IE-00-013 */
	 31'279'500, /* 31.2795_MHz_XTAL       Wyse WY-30+ */
	 31'684'000, /* 31.684_MHz_XTAL        TeleVideo TVI-955 132-column display clock */
	 31'948'800, /* 31.9488_MHz_XTAL       NEC PC-88xx, PC-98xx */
	 32'000'000, /* 32_MHz_XTAL            - */
	 32'147'000, /* 32.147_MHz_XTAL        Ampex 210+ 132-column display clock */
	 32'220'000, /* 32.22_MHz_XTAL         Typically used on 90's Data East PCBs (close to 9x NTSC subcarrier which is 32.215905Mhz */
	 32'317'400, /* 32.3174_MHz_XTAL       DEC VT330, VT340 */
	 32'530'400, /* 32.5304_MHz_XTAL       Seta 2 */
	 33'000'000, /* 33_MHz_XTAL            Sega Model 3 video board */
	 33'264'000, /* 33.264_MHz_XTAL        Hazeltine 1500 terminal */
	 33'333'000, /* 33.333_MHz_XTAL        Sega Model 3 CPU board, Vegas */
	 33'833'000, /* 33.833_MHz_XTAL        - */
	 33'868'800, /* 33.8688_MHz_XTAL       Usually used to drive 90's Yamaha OPL/FM chips with /2 divider */
	 34'000'000, /* 34_MHz_XTAL            Gaelco PCBs */
	 34'291'712, /* 34.291712_MHz_XTAL     Fairlight CMI master card */
	 34'846'000, /* 34.846_MHz_XTAL        Visual 550 */
	 35'904'000, /* 35.904_MHz_XTAL        Used on HP98543 graphics board */
	 36'000'000, /* 36_MHz_XTAL            Sega Model 1 video board */
	 37'980'000, /* 37.98_MHz_XTAL         Falco 5220 */
	 38'769'220, /* 38.76922_MHz_XTAL      Namco System 21 video board */
	 38'863'630, /* 38.86363_MHz_XTAL      Sharp X68000 15.98kHz video */
	 39'321'600, /* 39.3216_MHz_XTAL       Sun 2/120 */
	 39'710'000, /* 39.71_MHz_XTAL         Wyse WY-60 132-column display clock */
	 40'000'000, /* 40_MHz_XTAL            - */
	 40'210'000, /* 40.21_MHz_XTAL         Fairlight CMI IIx */
	 42'000'000, /* 42_MHz_XTAL            BMC A-00211 - Popo Bear */
	 42'105'200, /* 42.1052_MHz_XTAL       NEC PC-88xx */
	 42'954'545, /* 42.954545_MHz_XTAL     CPS3 (12x NTSC subcarrier)*/
	 43'320'000, /* 43.32_MHz_XTAL         DEC VT420 */
	 44'100'000, /* 44.1_MHz_XTAL          Subsino's Bishou Jan */
	 44'452'800, /* 44.4528_MHz_XTAL       TeleVideo 965 */
	 45'000'000, /* 45_MHz_XTAL            Eolith with Hyperstone CPUs */
	 45'158'000, /* 45.158_MHz_XTAL        Sega Model 2A video board, Model 3 CPU board */
	 45'619'200, /* 45.6192_MHz_XTAL       DEC VK100 */
	 45'830'400, /* 45.8304_MHz_XTAL       Microterm 5510 */
	 46'615'120, /* 46.61512_Mhz_XTAL      Soundblaster 16 PCM base clock */
	 47'736'000, /* 47.736_MHz_XTAL        Visual 100 */
	 48'000'000, /* 48_MHz_XTAL            Williams/Midway Y/Z-unit system / SSV board */
	 48'384'000, /* 48.384_MHz_XTAL        Namco NB-1 */
	 48'556'800, /* 48.5568_MHz_XTAL       Wyse WY-85 */
	 48'654'000, /* 48.654_MHz_XTAL        Qume QVT-201 */
	 48'660'000, /* 48.66_MHz_XTAL         Zaxxon */
	 49'152'000, /* 49.152_MHz_XTAL        Used on some Namco PCBs, Baraduke h/w, System 21, Super System 22 */
	 49'423'500, /* 49.4235_MHz_XTAL       Wyse WY-185 */
	 50'000'000, /* 50_MHz_XTAL            Williams/Midway T/W/V-unit system */
	 50'113'000, /* 50.113_MHz_XTAL        Namco NA-1 (14x NTSC subcarrier)*/
	 50'349'000, /* 50.349_MHz_XTAL        Sega System 18 (~3200x NTSC line rate) */
	 51'200'000, /* 51.2_MHz_XTAL          Namco Super System 22 video clock */
	 52'000'000, /* 52_MHz_XTAL            Cojag */
	 52'832'000, /* 52.832_MHz_XTAL        Wang PC TIG video controller */
	 53'203'400, /* 53.2034_MHz_XTAL       Master System, Mega Drive PAL (~12x PAL subcarrier) */
	 53'693'175, /* 53.693175_MHz_XTAL     PSX-based h/w, Sony ZN1-2-based (15x NTSC subcarrier) */
	 54'000'000, /* 54_MHz_XTAL            Taito JC */
	 55'000'000, /* 55_MHz_XTAL            Eolith Vega */
	 57'272'727, /* 57.272727_MHz_XTAL     Psikyo SH2 with /2 divider (16x NTSC subcarrier)*/
	 58'000'000, /* 58_MHz_XTAL            Magic Reel (Play System) */
	 59'292'000, /* 59.292_MHz_XTAL        Data General D461 */
	 60'000'000, /* 60_MHz_XTAL            ARM610 */
	 61'440'000, /* 61.44_MHz_XTAL         Donkey Kong */
	 64'000'000, /* 64_MHz_XTAL            BattleToads */
	 66'666'700, /* 66.6667_MHz_XTAL       Later Midway games */
	 67'737'600, /* 67.7376_MHz_XTAL       PSX-based h/w, Sony ZN1-2-based */
	 68'850'000, /* 68.85_MHz_XTAL         Wyse WY-50 */
	 69'551'990, /* 69.55199_MHz_XTAL      Sharp X68000 31.5kHz video */
	 72'000'000, /* 72_MHz_XTAL            Aristocrat MKV */
	 72'576'000, /* 72.576_MHz_XTAL        Centipede, Millipede, Missile Command, Let's Go Bowling "Multipede" */
	 73'728'000, /* 73.728_MHz_XTAL        Ms. Pac-Man/Galaga 20th Anniversary */
	 80'000'000, /* 80_MHz_XTAL            ARM710 */
	 87'183'360, /* 87.18336_MHz_XTAL      AT&T 630 MTG */
	100'000'000, /* 100_MHz_XTAL           PSX-based Namco System 12, Vegas, Sony ZN1-2-based */
	101'491'200, /* 101.4912_MHz_XTAL      PSX-based Namco System 10 */
	200'000'000  /* 200_MHz_XTAL           Base SH4 CPU (Naomi, Hikaru etc.) */
};

double XTAL::last_correct_value = -1;
double XTAL::xtal_error_low = 0;
double XTAL::xtal_error_high = 0;

bool XTAL::validate(double base_clock)
{
	if(base_clock == last_correct_value)
		return true;
	const unsigned int xtal_count = sizeof(known_xtals) / sizeof(known_xtals[0]);
	const unsigned int last_index = xtal_count - 1;
	const unsigned int fill1  = last_index | (last_index >> 1);
	const unsigned int fill2  = fill1 | (fill1 >> 2);
	const unsigned int fill4  = fill2 | (fill2 >> 4);
	const unsigned int fill8  = fill4 | (fill4 >> 8);
	const unsigned int fill16 = fill8 | (fill8 >> 16);
	const unsigned int ppow2  = fill16 - (fill16 >> 1);
	unsigned int slot = ppow2;
	unsigned int step = ppow2;
	while(step) {
		if(slot > last_index)
			slot = slot ^ (step | (step >> 1));
		else {
			const double sfreq = known_xtals[slot];
			const double diff = std::abs((base_clock - sfreq) / base_clock);
			if(diff <= (2 * DBL_EPSILON)) {
				last_correct_value = base_clock;
				return true;
			}
			if(base_clock > sfreq)
				slot = slot | (step >> 1);
			else
				slot = slot ^ (step | (step >> 1));
		}
		step = step >> 1;
	}

	double sfreq = known_xtals[slot];
	if(base_clock == sfreq) {
		last_correct_value = base_clock;
		return true;
	}

	if(base_clock < sfreq) {
		if(slot)
			xtal_error_low = known_xtals[slot-1];
		else
			xtal_error_low = 0;
		xtal_error_high = sfreq;
	} else {
		if(slot < last_index)
			xtal_error_high = known_xtals[slot+1];
		else
			xtal_error_high = 0;
		xtal_error_low = sfreq;
	}
	return false;
}

void XTAL::validate(const char *message) const
{
	if(!validate(m_base_clock))
		fail(m_base_clock, message);
}

void XTAL::validate(const std::string &message) const
{
	if(!validate(m_base_clock))
		fail(m_base_clock, message);
}

void XTAL::fail(double base_clock, std::string message)
{
	std::string full_message = util::string_format("Unknown crystal value %.0f. ", base_clock);
	if(xtal_error_low && xtal_error_high)
		full_message += util::string_format(" Did you mean %.0f or %.0f?", xtal_error_low, xtal_error_high);
	else
		full_message += util::string_format(" Did you mean %.0f?", xtal_error_low ? xtal_error_low : xtal_error_high);
	full_message += util::string_format(" Context: %s\n", message);
	fatalerror("%s\n", full_message.c_str());
}

/*

For further reference:

A search at http://search.digikey.com/scripts/DkSearch/dksus.dll?Cat=852333;keywords=cry
reveals the following shipping frequencies as of 1/1/2008:

20kHz
25.600kHz
26.667kHz
28kHz

30kHz
30.720kHz
30.76kHz
31.2kHz
31.25kHz
31.5kHz
32.000kHz
32.56kHz
32.768kHz
32.919kHz
34kHz
36kHz
38kHz
38.4kHz
39.500kHz

40kHz
44.100kHz
46.604kHz
46.6084kHz

50kHz
59.787kHz

60.000kHz
60.002kHz
60.005kHz
65.535kHz
65.536kHz
69kHz

70kHz
71kHz
72kHz
73kHz
74kHz
74.3kHz
74.4kHz
75kHz
76kHz
76.79kHz
76.8kHz
76.81kHz
77kHz
77.204kHz
77.287kHz
77.500kHz
77.503kHz
77.504kHz
78kHz
79kHz

83kHz

96kHz
96.006kHz

100kHz
111kHz
117.72kHz
120kHz
120.8475kHz
125kHz
131.072kHz
149.475kHz
153.600kHz

200kHz

307.2kHz

1.000MHz
1.8432MHz

2.000MHz
2.048MHz
2.097152MHz
2.4576MHz
2.5MHz
2.560MHz
2.949120MHz

3.000MHz
3.276MHz
3.2768MHz
3.579MHz
3.579545MHz
3.640MHz
3.6864MHz
3.700MHz
3.859MHz
3.93216MHz

4.000MHz
4.032MHz
4.096MHz
4.09625MHz
4.194MHz
4.194304MHz
4.332MHz
4.433MHz
4.433616MHz
4.433618MHz
4.433619MHz
4.74687MHz
4.800MHz
4.8970MHz
4.90625MHz
4.915MHz
4.9152MHz

5.000MHz
5.0688MHz
5.120MHz
5.223438MHz
5.5MHz
5.5296MHz
5.9904MHz

6.000MHz
6.14MHz
6.144MHz
6.1760MHz
6.400 MHz
6.49830MHz
6.5MHz
6.5536MHz
6.612813MHz
6.7458MHz
6.757MHz
6.76438MHz

7.1505MHz
7.15909 MHz
7.2MHz
7.3728MHz
7.68MHz
7.94888MHz

8.000MHz
8.000156MHz
8.192MHz
8.388608MHz
8.432MHz
8.5MHz
8.6432MHz

9.000MHz
9.216MHz
9.509375MHz
9.545MHz
9.6MHz
9.7941MHz
9.830MHz
9.8304MHz
9.84375MHz
9.8438MHz

10.000MHz
10.240MHz
10.245MHz
10.6244MHz
10.738635MHz
10.73865MHz

11.000MHz
11.046MHz
11.0592MHz
11.228MHz
11.2896MHz
11.520MHz
11.981350MHz

12.000MHz
12.000393MHz
12.096MHz
12.1875MHz
12.288MHz
12.352MHz
12.500MHz
12.688MHz
12.800MHz
12.96MHz

13.000MHz
13.0625MHz
13.225MHz
13.2256MHz
13.500MHz
13.5168MHz
13.56MHz
13.605MHz
13.824MHz
13.94916MHz

14.00MHz
14.318MHz
14.31818MHz
14.3359MHz
14.3594MHz
14.4MHz
14.5MHz
14.69MHz
14.7456MHz
14.850MHz

15MHz
15.360MHz

16.000MHz
16.000312MHz
16.128MHz
16.257MHz
16.3676MHz
16.368MHz
16.384MHz
16.576MHz
16.6660MHz
16.667MHz
16.670MHz
16.800MHz
16.934MHz
16.9344MHz

17.734475MHz

18.000MHz
18.432MHz
18.869MHz

19.200MHz
19.440MHz
19.660MHz
19.6608MHz
19.68MHz
19.800MHz

20.000MHz
20.35625MHz
20.3563MHz
20.480MHz

21.47727MHz

22.000MHz
22.118MHz
22.1184MHz
22.400MHz
22.5MHz
22.5792MHz
22.6278MHz

23MHz
23.2643MHz
23.5MHz
23.5122MHz
23.592MHz

24.000MHz
24.00014MHz
24.5MHz
24.545454 MHz
24.5535MHz
24.576MHz
24.704MHz
24.7456MHz

25.000MHz
25MHz
25.175MHz
25.2235MHz
25.4563MHz
25.5MHz

26.000MHz
26.45125MHz
26.4513MHz
26.5MHz
26.5971MHz
26.800MHz

27.000MHz
27.1344MHz
27.3067MHz
27.4688MHz

28.000MHz
28.224MHz
28.259375MHz
28.2594MHz
28.322MHz
28.375MHz
28.5938MHz
28.636MHz
28.6363MHz
28.63636MHz

29.4912MHz
29.498928MHz
29.500MHz

30.000MHz
32.000MHz
32.514MHz
32.768MHz
33.000MHz
33.333MHz
33.3333MHz
33.8688MHz
35.2512MHz
35.3280MHz
36.000MHz
38.000MHz
38.00053MHz
38.400MHz
38.880MHz
39MHz

40.000MHz
40.320MHz
40.960 MHz
42.000MHz
44.000MHz
44.2368MHz
44.545MHz
44.736MHz
44.800MHz
44.900MHz
45.000MHz
46.000MHz
48.000MHz
49.152MHz
49.86MHz

50.000MHz
53.125MHz
55.000MHz

60.000MHz
64.000MHz
66.000MHz
66.666MHz
66.6666MHz

73.66979MHz
75.957292MHz
76.121875MHz

80.000MHz

100.00MHz

*/
