#pragma once
#include <bitset>
#include <string>
#include <vector>
#include <array>

#include "Common.h"
#include "Logger/Logger.h"

namespace tur
{
	class CPU_Information
	{
	public:
		explicit CPU_Information();

	private:
		void prepareVendor();

		void prepareFunctions();

	public:
		std::string vendor() const { return m_Vendor; }

		std::string brand() const;

		bool isAMD() const { return m_IsAMD; }

		bool isIntel() const { return m_IsIntel; }

		// Function: [CPUID EAX=1] - EDX
	public:
		bool hasFPU()				const { return m_Function_eax1_EDX[0] || hasFPU_AMD(); };
		bool hasVME()				const { return m_Function_eax1_EDX[1] || hasVME_AMD(); };
		bool hasDebugExtensions()	const { return m_Function_eax1_EDX[2] || hasDebugExtensions_AMD(); };
		bool hasPSE()				const { return m_Function_eax1_EDX[3] || hasPSE_AMD(); };
		bool hasRDTSC()				const { return m_Function_eax1_EDX[4] || hasRDTSC_AMD(); };
		bool hasMSR()				const { return m_Function_eax1_EDX[5] || hasMSR_AMD(); };
		bool hasPAE()				const { return m_Function_eax1_EDX[6] || hasPAE_AMD(); };
		bool hasMCE()				const { return m_Function_eax1_EDX[7] || hasMCE_AMD(); };
		bool hasCX8()				const { return m_Function_eax1_EDX[8] || hasCX8_AMD(); };
		bool hasAPIC()				const { return m_Function_eax1_EDX[9] || hasAPIC_AMD(); };
		// bool reserved()			const { return m_Function_eax1_EDX[10]; };
		bool hasSEP()				const { return m_Function_eax1_EDX[11]; };
		bool hasMTRR()				const { return m_Function_eax1_EDX[12] || hasMTRR_AMD(); };
		bool hasPGE()				const { return m_Function_eax1_EDX[13] || hasPGE_AMD(); };
		bool hasMCA()				const { return m_Function_eax1_EDX[14] || hasMCA_AMD(); };
		bool hasCMOV()				const { return m_Function_eax1_EDX[15] || hasCMOV_AMD(); };
		bool hasPAT()				const { return m_Function_eax1_EDX[16] || hasPAT_AMD(); };
		bool hasPSE36()				const { return m_Function_eax1_EDX[17] || hasPSE36_AMD(); };
		bool hasPSN()				const { return m_Function_eax1_EDX[18]; };
		bool hasCLFLUSH()			const { return m_Function_eax1_EDX[19]; };
		bool hasNX()				const { return m_Function_eax1_EDX[20]; };
		bool hasDS()				const { return m_Function_eax1_EDX[21]; };
		bool hasACPI()				const { return m_Function_eax1_EDX[22]; };
		bool hasMMX()				const { return m_Function_eax1_EDX[23] || hasMMX_AMD(); };
		bool hasFXSR()				const { return m_Function_eax1_EDX[24] || hasFXSR_AMD(); };
		bool hasSSE()				const { return m_Function_eax1_EDX[25]; };
		bool hasSSE2()				const { return m_Function_eax1_EDX[26]; };
		bool hasSS()				const { return m_Function_eax1_EDX[27]; };
		bool hasHTT()				const { return m_Function_eax1_EDX[28]; };
		bool hasTM()				const { return m_Function_eax1_EDX[29]; };
		bool hasIA64()				const { return m_Function_eax1_EDX[30]; };
		bool hasPBE()				const { return m_Function_eax1_EDX[31]; };

		// Function: [CPUID EAX=1] - ECX
		bool hasSSE3()				const { return m_Function_eax1_ECX[0]; };
		bool hasPCLMULQDQ()			const { return m_Function_eax1_ECX[1]; };
		bool hasDTES64()			const { return m_Function_eax1_ECX[2]; };
		bool hasMonitor()			const { return m_Function_eax1_ECX[3]; };
		bool hasDSCPL()				const { return m_Function_eax1_ECX[4]; };
		bool hasVMX()				const { return m_Function_eax1_ECX[5]; };
		bool hasSMX()				const { return m_Function_eax1_ECX[6]; };
		bool hasEST()				const { return m_Function_eax1_ECX[7]; };
		bool hasTM2()				const { return m_Function_eax1_ECX[8]; };
		bool hasS_SSE3()			const { return m_Function_eax1_ECX[9]; };
		bool hasL1_CTX()			const { return m_Function_eax1_ECX[10]; };
		bool hasSDBG()				const { return m_Function_eax1_ECX[11]; };
		bool hasFMA()				const { return m_Function_eax1_ECX[12]; };
		bool hasCX16()				const { return m_Function_eax1_ECX[13]; };
		bool hasXTPR()				const { return m_Function_eax1_ECX[14]; };
		bool hasPDCM()				const { return m_Function_eax1_ECX[15]; };
		//bool reserved()			const { return m_Function_eax1_ECX[16]; };
		bool hasPCID()				const { return m_Function_eax1_ECX[17]; };
		bool hasDCA()				const { return m_Function_eax1_ECX[18]; };
		bool hasSSE4_1()			const { return m_Function_eax1_ECX[19]; };
		bool hasSSE4_2()			const { return m_Function_eax1_ECX[20]; };
		bool hasX2APIC()			const { return m_Function_eax1_ECX[21]; };
		bool hasMOVBE()				const { return m_Function_eax1_ECX[22]; };
		bool hasPOPCNT()			const { return m_Function_eax1_ECX[23]; };
		bool hasTSC_Deadline()		const { return m_Function_eax1_ECX[24]; };
		bool hasAES_NI()			const { return m_Function_eax1_ECX[25]; };
		bool hasXSAVE()				const { return m_Function_eax1_ECX[26]; };
		bool hasOSXSAVE()			const { return m_Function_eax1_ECX[27]; };
		bool hasAVX()				const { return m_Function_eax1_ECX[28]; };
		bool hasF16C()				const { return m_Function_eax1_ECX[29]; };
		bool hasRDRAND()			const { return m_Function_eax1_ECX[30]; };
		bool hasHyperVisor()		const { return m_Function_eax1_ECX[31]; };

		// Function: [CPUID EAX=7, ECX=0] - EBX
	public:
		bool hasFSGSBASE()			const { return m_Function_eax7_ecx0_EBX[0]; };
		bool IA32_TSC_ADJUST()		const { return m_Function_eax7_ecx0_EBX[1]; };
		bool hasSGX()				const { return m_Function_eax7_ecx0_EBX[2]; };
		bool hasBMI1()				const { return m_Function_eax7_ecx0_EBX[3]; };
		bool hasHLE()				const { return m_Function_eax7_ecx0_EBX[4]; };
		bool hasAVX2()				const { return m_Function_eax7_ecx0_EBX[5]; };
		bool hasFPUx87()			const { return m_Function_eax7_ecx0_EBX[6]; };
		bool hasSMEP()				const { return m_Function_eax7_ecx0_EBX[7]; };
		bool hasBMI2()				const { return m_Function_eax7_ecx0_EBX[8]; };
		bool hasERMS()				const { return m_Function_eax7_ecx0_EBX[9]; };
		bool hasINVPCID()			const { return m_Function_eax7_ecx0_EBX[10]; };
		bool hasRTM()				const { return m_Function_eax7_ecx0_EBX[11]; };
		bool hasRDT_M()				const { return m_Function_eax7_ecx0_EBX[12]; };
		//bool hasINVPCID()			const { return m_Function_eax7_ecx0_EBX[13]; }; //	x87 FPU CS and DS deprecated
		bool hasMPX()				const { return m_Function_eax7_ecx0_EBX[14]; };
		bool hasRDT_A_PQE()			const { return m_Function_eax7_ecx0_EBX[15]; };
		bool hasAVX512_F()			const { return m_Function_eax7_ecx0_EBX[16]; };
		bool hasAVX512_DQ()			const { return m_Function_eax7_ecx0_EBX[17]; };
		bool hasRDSEED()			const { return m_Function_eax7_ecx0_EBX[18]; };
		bool hasADX()				const { return m_Function_eax7_ecx0_EBX[19]; };
		bool hasSMAP()				const { return m_Function_eax7_ecx0_EBX[20]; };
		bool hasAVX512_IFMA()		const { return m_Function_eax7_ecx0_EBX[21]; };
		bool hasPCOMMIT()			const { return m_Function_eax7_ecx0_EBX[22]; };
		bool hasCLFLUSHOPT()		const { return m_Function_eax7_ecx0_EBX[23]; };
		bool hasCLWB()				const { return m_Function_eax7_ecx0_EBX[24]; };
		bool hasPT()				const { return m_Function_eax7_ecx0_EBX[25]; };
		bool hasAVX512_PF()			const { return m_Function_eax7_ecx0_EBX[26]; };
		bool hasAVX512_ER()			const { return m_Function_eax7_ecx0_EBX[27]; };
		bool hasAVX512_CD()			const { return m_Function_eax7_ecx0_EBX[28]; };
		bool hasSHA()				const { return m_Function_eax7_ecx0_EBX[29]; };
		bool hasAVX512_BW()			const { return m_Function_eax7_ecx0_EBX[30]; };
		bool hasAVX512_VL()			const { return m_Function_eax7_ecx0_EBX[31]; };

		// Function: [CPUID EAX=7, ECX=0] - ECX
		bool hasPREFETCHWT1()		const { return m_Function_eax7_ecx0_ECX[0]; };
		bool hasAVX512_VBMI()		const { return m_Function_eax7_ecx0_ECX[1]; };
		bool hasUMIP()				const { return m_Function_eax7_ecx0_ECX[2]; };
		bool hasPKU()				const { return m_Function_eax7_ecx0_ECX[3]; };
		bool hasOSPKE()				const { return m_Function_eax7_ecx0_ECX[4]; };
		bool hasWAITPKG()			const { return m_Function_eax7_ecx0_ECX[5]; };
		bool hasAVX512_VBMI_2()		const { return m_Function_eax7_ecx0_ECX[6]; };
		bool hasCET_SHSTK()			const { return m_Function_eax7_ecx0_ECX[7]; };
		bool hasGFNI()				const { return m_Function_eax7_ecx0_ECX[8]; };
		bool hasVAES()				const { return m_Function_eax7_ecx0_ECX[9]; };
		bool hasCLMUL()				const { return m_Function_eax7_ecx0_ECX[10]; };
		bool hasAVX512_VNNI()		const { return m_Function_eax7_ecx0_ECX[11]; };
		bool hasAVX512_BITALG()		const { return m_Function_eax7_ecx0_ECX[12]; };
		bool hasTME()				const { return m_Function_eax7_ecx0_ECX[13]; };
		bool hasAVX512_VPOPCNTDQ()	const { return m_Function_eax7_ecx0_ECX[14]; };
		//bool reserved()			const { return m_Function_eax7_ecx0_ECX[15]; };
		bool hasLA57()				const { return m_Function_eax7_ecx0_ECX[16]; };
		bool mawau0()				const { return m_Function_eax7_ecx0_ECX[17]; };
		bool mawau1()				const { return m_Function_eax7_ecx0_ECX[18]; };
		bool mawau2()				const { return m_Function_eax7_ecx0_ECX[19]; };
		bool mawau3()				const { return m_Function_eax7_ecx0_ECX[20]; };
		bool mawau4()				const { return m_Function_eax7_ecx0_ECX[21]; };
		bool hasRDPID()				const { return m_Function_eax7_ecx0_ECX[22]; };
		bool hasAES_KL()			const { return m_Function_eax7_ecx0_ECX[23]; };
		bool hasBusLockDetect()		const { return m_Function_eax7_ecx0_ECX[24]; };
		bool hasCLDEMOTE()			const { return m_Function_eax7_ecx0_ECX[25]; };
		//bool reserved()			const { return m_Function_eax7_ecx0_ECX[26]; };
		bool hasMOVDIRI()			const { return m_Function_eax7_ecx0_ECX[27]; };
		bool hasMOVDIR64B()			const { return m_Function_eax7_ecx0_ECX[28]; };
		bool hasEMQCMD()			const { return m_Function_eax7_ecx0_ECX[29]; };
		bool hasSGX_LC()			const { return m_Function_eax7_ecx0_ECX[30]; };
		bool hasPKS()				const { return m_Function_eax7_ecx0_ECX[31]; };

		// Function: [CPUID EAX=7, ECX=0] - EDX
			//bool reserved()				const { return m_Function_eax7_ecx0_EDX[ 0]; };
		bool hasSGX_KEYS()				const { return m_Function_eax7_ecx0_EDX[1]; };
		bool hasAVX512_4vnniw()			const { return m_Function_eax7_ecx0_EDX[2]; };
		bool hasAVX512_4maps()			const { return m_Function_eax7_ecx0_EDX[3]; };
		bool hasFSRM()					const { return m_Function_eax7_ecx0_EDX[4]; };
		bool hasUINTR()					const { return m_Function_eax7_ecx0_EDX[5]; };
		//bool reserved()				const { return m_Function_eax7_ecx0_EDX[ 6]; };
		//bool reserved()				const { return m_Function_eax7_ecx0_EDX[ 7]; };
		bool hasAVX512_VP2intersect()	const { return m_Function_eax7_ecx0_EDX[8]; };
		bool hasSRDBS_CTRL()			const { return m_Function_eax7_ecx0_EDX[9]; };
		bool hasMC_CLEAR()				const { return m_Function_eax7_ecx0_EDX[10]; };
		bool TSXAlwaysAbort()			const { return m_Function_eax7_ecx0_EDX[11]; };
		//bool reserved()				const { return m_Function_eax7_ecx0_EDX[12]; };
		bool TSX_FORCE_ABORT_MSR()		const { return m_Function_eax7_ecx0_EDX[13]; };
		bool hasSERIALIZE()				const { return m_Function_eax7_ecx0_EDX[14]; };
		bool isHybrid()					const { return m_Function_eax7_ecx0_EDX[15]; };
		bool hasTSKLDTRK()				const { return m_Function_eax7_ecx0_EDX[16]; };
		//bool reserved()				const { return m_Function_eax7_ecx0_EDX[17]; };
		bool hasPCONFIG()				const { return m_Function_eax7_ecx0_EDX[18]; };
		bool hasLBR()					const { return m_Function_eax7_ecx0_EDX[19]; };
		bool hasCET_IBT()				const { return m_Function_eax7_ecx0_EDX[20]; };
		//bool reserved()				const { return m_Function_eax7_ecx0_EDX[21]; };
		bool hasAMXbf16()				const { return m_Function_eax7_ecx0_EDX[22]; };
		bool hasAVX512_fp16()			const { return m_Function_eax7_ecx0_EDX[23]; };
		bool hasAMX_TILE()				const { return m_Function_eax7_ecx0_EDX[24]; };
		bool hasAMX_int8()				const { return m_Function_eax7_ecx0_EDX[25]; };
		bool hasIBRS_IBPB()				const { return m_Function_eax7_ecx0_EDX[26]; };
		bool hasSTIBP()					const { return m_Function_eax7_ecx0_EDX[27]; };
		bool hasL1D_FLUSH()				const { return m_Function_eax7_ecx0_EDX[28]; };
		bool IA32_ARCH_CAPABILITIES()	const { return m_Function_eax7_ecx0_EDX[29]; };
		bool IA32_CORE_CAPABILITIES()	const { return m_Function_eax7_ecx0_EDX[30]; };
		bool hasSSBD()					const { return m_Function_eax7_ecx0_EDX[31]; };


		// Function: [CPUID EAX=7, ECX=1] - EAX
			// bool reserved()				const { return m_Function_eax7_ecx1_EAX[ 0]; };
			// bool reserved()				const { return m_Function_eax7_ecx1_EAX[ 1]; };
			// bool reserved()				const { return m_Function_eax7_ecx1_EAX[ 2]; };
		bool hasRAO_INT()				const { return m_Function_eax7_ecx1_EAX[3]; };
		bool hasAVX_ANNI()				const { return m_Function_eax7_ecx1_EAX[4]; };
		bool hasAVX512_bf16()			const { return m_Function_eax7_ecx1_EAX[5]; };
		bool hasLASS()					const { return m_Function_eax7_ecx1_EAX[6]; };
		bool hasCMPCCXADD()				const { return m_Function_eax7_ecx1_EAX[7]; };
		bool hasARCHPERFMONEXT()		const { return m_Function_eax7_ecx1_EAX[8]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EAX[ 9]; };
		bool fast_zero_rep_movsb()			const { return m_Function_eax7_ecx1_EAX[10]; };
		bool fast_short_rep_stosb()			const { return m_Function_eax7_ecx1_EAX[11]; };
		bool fast_short_rep_cmpsb_scasb()	const { return m_Function_eax7_ecx1_EAX[12]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EAX[13]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EAX[14]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EAX[15]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EAX[16]; };
		bool hasFRED()					const { return m_Function_eax7_ecx1_EAX[17]; };
		bool hasLKGS()					const { return m_Function_eax7_ecx1_EAX[18]; };
		bool hasWRMSRNS()				const { return m_Function_eax7_ecx1_EAX[19]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EAX[20]; };
		bool hasAMX_fp16()				const { return m_Function_eax7_ecx1_EAX[21]; };
		bool has_hreset()				const { return m_Function_eax7_ecx1_EAX[22]; };
		bool hasAVX_ifma()				const { return m_Function_eax7_ecx1_EAX[23]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EAX[24]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EAX[25]; };
		bool hasLAM()					const { return m_Function_eax7_ecx1_EAX[26]; };
		bool has_MSRLIST()				const { return m_Function_eax7_ecx1_EAX[27]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EAX[28]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EAX[29]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EAX[30]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EAX[31]; };

	// Function: [CPUID EAX=7, ECX=1] - EBX
	public:
		bool IA32_PPIN()				const { return m_Function_eax7_ecx1_EBX[0]; };


		// Function: [CPUID EAX=7, ECX=1] - EDX [Continue]
			// bool reserved()				const { return m_Function_eax7_ecx1_EDX[ 0]; };
			// bool reserved()				const { return m_Function_eax7_ecx1_EDX[ 1]; };
			// bool reserved()				const { return m_Function_eax7_ecx1_EDX[ 2]; };
			// bool reserved()				const { return m_Function_eax7_ecx1_EDX[ 3]; };
		bool hasAVX_VNN_int8()			const { return m_Function_eax7_ecx1_EDX[4]; };
		bool hasAVX_ne_convert()		const { return m_Function_eax7_ecx1_EDX[5]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EDX[ 6]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EDX[ 7]; };
		bool hasAMX_COMPLEX()			const { return m_Function_eax7_ecx1_EDX[8]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EDX[ 9]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EDX[10]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EDX[11]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EDX[12]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EDX[13]; };
		bool hasPREFETCHI()				const { return m_Function_eax7_ecx1_EDX[14]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EDX[15]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EDX[16]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EDX[17]; };
		bool hasCETSSS()				const { return m_Function_eax7_ecx1_EDX[18]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EDX[19]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EDX[20]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EDX[21]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EDX[22]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EDX[23]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EDX[24]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EDX[25]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EDX[26]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EDX[27]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EDX[28]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EDX[29]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EDX[30]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EDX[31]; };

	// Function: [CPUID EAX=7, ECX=2] - EDX
	public:
		bool hasPFSD()					const { return m_Function_eax7_ecx2_EDX[0]; };
		bool hasIPRED_DIS()				const { return m_Function_eax7_ecx2_EDX[1]; };
		bool hasRRSBA()					const { return m_Function_eax7_ecx2_EDX[2]; };
		bool hasDPPD_U()				const { return m_Function_eax7_ecx2_EDX[3]; };
		bool hasBHI_DIS_S()				const { return m_Function_eax7_ecx2_EDX[4]; };
		bool hasMCDT_NO()				const { return m_Function_eax7_ecx2_EDX[5]; };

		// Function: [CPUID EAX=80000001h] - EDX
	public:
		bool hasFPU_AMD()				const { return m_Function_81h_EDX[0] || hasFPU_AMD(); };
		bool hasVME_AMD()				const { return m_Function_81h_EDX[1]; };
		bool hasDebugExtensions_AMD()	const { return m_Function_81h_EDX[2]; };
		bool hasPSE_AMD()				const { return m_Function_81h_EDX[3]; };
		bool hasRDTSC_AMD()				const { return m_Function_81h_EDX[4]; };
		bool hasMSR_AMD()				const { return m_Function_81h_EDX[5]; };
		bool hasPAE_AMD()				const { return m_Function_81h_EDX[6]; };
		bool hasMCE_AMD()				const { return m_Function_81h_EDX[7]; };
		bool hasCX8_AMD()				const { return m_Function_81h_EDX[8]; };
		bool hasAPIC_AMD()				const { return m_Function_81h_EDX[9]; };
		bool syscall_K6()				const { return m_Function_81h_EDX[10]; };
		bool syscall()					const { return m_Function_81h_EDX[11]; };
		bool hasMTRR_AMD()				const { return m_Function_81h_EDX[12]; };
		bool hasPGE_AMD()				const { return m_Function_81h_EDX[13]; };
		bool hasMCA_AMD()				const { return m_Function_81h_EDX[14]; };
		bool hasCMOV_AMD()				const { return m_Function_81h_EDX[15]; };
		bool hasPAT_AMD()				const { return m_Function_81h_EDX[16]; };
		bool hasPSE36_AMD()				const { return m_Function_81h_EDX[17]; };
		//bool reserved()				const { return m_Function_81h_EDX[18]; };
		bool hasECC()					const { return m_Function_81h_EDX[19]; };
		bool hasNX_AMD()				const { return m_Function_81h_EDX[20]; };
		//bool reserved()				const { return m_Function_81h_EDX[21]; };
		bool hasMMEXT()					const { return m_Function_81h_EDX[22]; };
		bool hasMMX_AMD()				const { return m_Function_81h_EDX[23]; };
		bool hasFXSR_AMD()				const { return m_Function_81h_EDX[24]; };
		bool hasFXSR_OPT()				const { return m_Function_81h_EDX[25]; };
		bool hasPDPE1GB()				const { return m_Function_81h_EDX[26]; };
		bool hasRDTSCP()				const { return m_Function_81h_EDX[27]; };
		//bool reserved()				const { return m_Function_81h_EDX[28]; };
		bool hasLM()					const { return m_Function_81h_EDX[29]; };
		bool has3DNOWEXT()				const { return m_Function_81h_EDX[30]; };
		bool has3DNOW()					const { return m_Function_81h_EDX[31]; };

		// Function: [CPUID EAX=80000001h] - ECX
	public:
		bool hasLAHF_LM()					const { return m_Function_81h_ECX[0]; };
		bool hasCMP_LEGACY()				const { return m_Function_81h_ECX[1]; };
		bool hasSVM()						const { return m_Function_81h_ECX[2]; };
		bool hasEXTAPIC()					const { return m_Function_81h_ECX[3]; };
		bool hasCR8_LEGACY()				const { return m_Function_81h_ECX[4]; };
		bool hasABM()						const { return m_Function_81h_ECX[5]; };
		bool hasSSE4A()						const { return m_Function_81h_ECX[6]; };
		bool hassMISALIGNSSE()				const { return m_Function_81h_ECX[7]; };
		bool has3DNOWPREFETCH()				const { return m_Function_81h_ECX[8]; };
		bool hasOSVW()						const { return m_Function_81h_ECX[9]; };
		bool hasIBS()						const { return m_Function_81h_ECX[10]; };
		bool hasXOP()						const { return m_Function_81h_ECX[11]; };
		bool hasSKINIT()					const { return m_Function_81h_ECX[12]; };
		bool hasWDT()						const { return m_Function_81h_ECX[13]; };
		// bool reserved()					const { return m_Function_81h_ECX[14]; };
		bool hasLWP()						const { return m_Function_81h_ECX[15]; };
		bool hasFMA4()						const { return m_Function_81h_ECX[16]; };
		bool hasTCE()						const { return m_Function_81h_ECX[17]; };
		// bool reserved()					const { return m_Function_81h_ECX[18]; };
		bool hasNODEID_MSR()				const { return m_Function_81h_ECX[19]; };
		// bool reserved()					const { return m_Function_81h_ECX[20]; };
		bool hasTBM()						const { return m_Function_81h_ECX[21]; };
		bool hasTOPOEXT()					const { return m_Function_81h_ECX[22]; };
		bool hasPERFCTR_CORE()				const { return m_Function_81h_ECX[23]; };
		bool hasPERFCTR_NB()				const { return m_Function_81h_ECX[24]; };
		//bool reserved()					const { return m_Function_81h_ECX[25]; };
		bool hasDBX()						const { return m_Function_81h_ECX[26]; };
		bool hasPERFTSC()					const { return m_Function_81h_ECX[27]; };
		bool hasPCX_L2I()					const { return m_Function_81h_ECX[28]; };
		bool hasMONITORX()					const { return m_Function_81h_ECX[29]; };
		bool hasADDR_MASK_EXT()				const { return m_Function_81h_ECX[30]; };
		//bool reserved()					const { return m_Function_81h_ECX[31]; };

	private:
		std::string m_Vendor, m_Brand;

		std::bitset<32> m_Function_eax1_ECX, m_Function_eax1_EDX;
		std::bitset<32> m_Function_eax7_ecx0_EBX, m_Function_eax7_ecx0_ECX, m_Function_eax7_ecx0_EDX;
		std::bitset<32> m_Function_eax7_ecx1_EAX, m_Function_eax7_ecx1_EBX, m_Function_eax7_ecx1_EDX;
		std::bitset<32> m_Function_eax7_ecx2_EDX;
		std::bitset<32> m_Function_81h_ECX, m_Function_81h_EDX;

		bool m_IsAMD = false;
		bool m_IsIntel = false;
	};
}

namespace tur
{
	void display_cpu_information();
}