#pragma once
#include <bitset>
#include "Common.hpp"

namespace tur
{
	class CPU_Information
	{
	public:
		explicit CPU_Information();

	private:
		void PrepareVendor();

		void PrepareFunctions();

	public:
		std::string Vendor() const { return m_Vendor; }

		std::string Brand() const;

		bool IsAMD() const { return m_IsAMD; }

		bool IsIntel() const { return m_IsIntel; }

		// Function: [CPUID EAX=1] - EDX
	public:
		bool HasFPU()				const { return m_Function_eax1_EDX[0] || HasFPU_AMD(); };
		bool HasVME()				const { return m_Function_eax1_EDX[1] || HasVME_AMD(); };
		bool HasDebugExtensions()	const { return m_Function_eax1_EDX[2] || HasDebugExtensions_AMD(); };
		bool HasPSE()				const { return m_Function_eax1_EDX[3] || HasPSE_AMD(); };
		bool HasRDTSC()				const { return m_Function_eax1_EDX[4] || HasRDTSC_AMD(); };
		bool HasMSR()				const { return m_Function_eax1_EDX[5] || HasMSR_AMD(); };
		bool HasPAE()				const { return m_Function_eax1_EDX[6] || HasPAE_AMD(); };
		bool HasMCE()				const { return m_Function_eax1_EDX[7] || HasMCE_AMD(); };
		bool HasCX8()				const { return m_Function_eax1_EDX[8] || HasCX8_AMD(); };
		bool HasAPIC()				const { return m_Function_eax1_EDX[9] || HasAPIC_AMD(); };
		// bool reserved()			const { return m_Function_eax1_EDX[10]; };
		bool HasSEP()				const { return m_Function_eax1_EDX[11]; };
		bool HasMTRR()				const { return m_Function_eax1_EDX[12] || HasMTRR_AMD(); };
		bool HasPGE()				const { return m_Function_eax1_EDX[13] || HasPGE_AMD(); };
		bool HasMCA()				const { return m_Function_eax1_EDX[14] || HasMCA_AMD(); };
		bool HasCMOV()				const { return m_Function_eax1_EDX[15] || HasCMOV_AMD(); };
		bool HasPAT()				const { return m_Function_eax1_EDX[16] || HasPAT_AMD(); };
		bool HasPSE36()				const { return m_Function_eax1_EDX[17] || HasPSE36_AMD(); };
		bool HasPSN()				const { return m_Function_eax1_EDX[18]; };
		bool HasCLFLUSH()			const { return m_Function_eax1_EDX[19]; };
		bool HasNX()				const { return m_Function_eax1_EDX[20]; };
		bool HasDS()				const { return m_Function_eax1_EDX[21]; };
		bool HasACPI()				const { return m_Function_eax1_EDX[22]; };
		bool HasMMX()				const { return m_Function_eax1_EDX[23] || HasMMX_AMD(); };
		bool HasFXSR()				const { return m_Function_eax1_EDX[24] || HasFXSR_AMD(); };
		bool HasSSE()				const { return m_Function_eax1_EDX[25]; };
		bool HasSSE2()				const { return m_Function_eax1_EDX[26]; };
		bool HasSS()				const { return m_Function_eax1_EDX[27]; };
		bool HasHTT()				const { return m_Function_eax1_EDX[28]; };
		bool HasTM()				const { return m_Function_eax1_EDX[29]; };
		bool HasIA64()				const { return m_Function_eax1_EDX[30]; };
		bool HasPBE()				const { return m_Function_eax1_EDX[31]; };

		// Function: [CPUID EAX=1] - ECX
		bool HasSSE3()				const { return m_Function_eax1_ECX[0]; };
		bool HasPCLMULQDQ()			const { return m_Function_eax1_ECX[1]; };
		bool HasDTES64()			const { return m_Function_eax1_ECX[2]; };
		bool HasMonitor()			const { return m_Function_eax1_ECX[3]; };
		bool HasDSCPL()				const { return m_Function_eax1_ECX[4]; };
		bool HasVMX()				const { return m_Function_eax1_ECX[5]; };
		bool HasSMX()				const { return m_Function_eax1_ECX[6]; };
		bool HasEST()				const { return m_Function_eax1_ECX[7]; };
		bool HasTM2()				const { return m_Function_eax1_ECX[8]; };
		bool HasS_SSE3()			const { return m_Function_eax1_ECX[9]; };
		bool HasL1_CTX()			const { return m_Function_eax1_ECX[10]; };
		bool HasSDBG()				const { return m_Function_eax1_ECX[11]; };
		bool HasFMA()				const { return m_Function_eax1_ECX[12]; };
		bool HasCX16()				const { return m_Function_eax1_ECX[13]; };
		bool HasXTPR()				const { return m_Function_eax1_ECX[14]; };
		bool HasPDCM()				const { return m_Function_eax1_ECX[15]; };
		//bool reserved()			const { return m_Function_eax1_ECX[16]; };
		bool HasPCID()				const { return m_Function_eax1_ECX[17]; };
		bool HasDCA()				const { return m_Function_eax1_ECX[18]; };
		bool HasSSE4_1()			const { return m_Function_eax1_ECX[19]; };
		bool HasSSE4_2()			const { return m_Function_eax1_ECX[20]; };
		bool HasX2APIC()			const { return m_Function_eax1_ECX[21]; };
		bool HasMOVBE()				const { return m_Function_eax1_ECX[22]; };
		bool HasPOPCNT()			const { return m_Function_eax1_ECX[23]; };
		bool HasTSC_Deadline()		const { return m_Function_eax1_ECX[24]; };
		bool HasAES_NI()			const { return m_Function_eax1_ECX[25]; };
		bool HasXSAVE()				const { return m_Function_eax1_ECX[26]; };
		bool HasOSXSAVE()			const { return m_Function_eax1_ECX[27]; };
		bool HasAVX()				const { return m_Function_eax1_ECX[28]; };
		bool HasF16C()				const { return m_Function_eax1_ECX[29]; };
		bool HasRDRAND()			const { return m_Function_eax1_ECX[30]; };
		bool HasHyperVisor()		const { return m_Function_eax1_ECX[31]; };

		// Function: [CPUID EAX=7, ECX=0] - EBX
	public:
		bool HasFSGSBASE()			const { return m_Function_eax7_ecx0_EBX[0]; };
		bool IA32_TSC_ADJUST()		const { return m_Function_eax7_ecx0_EBX[1]; };
		bool HasSGX()				const { return m_Function_eax7_ecx0_EBX[2]; };
		bool HasBMI1()				const { return m_Function_eax7_ecx0_EBX[3]; };
		bool HasHLE()				const { return m_Function_eax7_ecx0_EBX[4]; };
		bool HasAVX2()				const { return m_Function_eax7_ecx0_EBX[5]; };
		bool HasFPUx87()			const { return m_Function_eax7_ecx0_EBX[6]; };
		bool HasSMEP()				const { return m_Function_eax7_ecx0_EBX[7]; };
		bool HasBMI2()				const { return m_Function_eax7_ecx0_EBX[8]; };
		bool HasERMS()				const { return m_Function_eax7_ecx0_EBX[9]; };
		bool HasINVPCID()			const { return m_Function_eax7_ecx0_EBX[10]; };
		bool HasRTM()				const { return m_Function_eax7_ecx0_EBX[11]; };
		bool HasRDT_M()				const { return m_Function_eax7_ecx0_EBX[12]; };
		//bool HasINVPCID()			const { return m_Function_eax7_ecx0_EBX[13]; }; //	x87 FPU CS and DS deprecated
		bool HasMPX()				const { return m_Function_eax7_ecx0_EBX[14]; };
		bool HasRDT_A_PQE()			const { return m_Function_eax7_ecx0_EBX[15]; };
		bool HasAVX512_F()			const { return m_Function_eax7_ecx0_EBX[16]; };
		bool HasAVX512_DQ()			const { return m_Function_eax7_ecx0_EBX[17]; };
		bool HasRDSEED()			const { return m_Function_eax7_ecx0_EBX[18]; };
		bool HasADX()				const { return m_Function_eax7_ecx0_EBX[19]; };
		bool HasSMAP()				const { return m_Function_eax7_ecx0_EBX[20]; };
		bool HasAVX512_IFMA()		const { return m_Function_eax7_ecx0_EBX[21]; };
		bool HasPCOMMIT()			const { return m_Function_eax7_ecx0_EBX[22]; };
		bool HasCLFLUSHOPT()		const { return m_Function_eax7_ecx0_EBX[23]; };
		bool HasCLWB()				const { return m_Function_eax7_ecx0_EBX[24]; };
		bool HasPT()				const { return m_Function_eax7_ecx0_EBX[25]; };
		bool HasAVX512_PF()			const { return m_Function_eax7_ecx0_EBX[26]; };
		bool HasAVX512_ER()			const { return m_Function_eax7_ecx0_EBX[27]; };
		bool HasAVX512_CD()			const { return m_Function_eax7_ecx0_EBX[28]; };
		bool HasSHA()				const { return m_Function_eax7_ecx0_EBX[29]; };
		bool HasAVX512_BW()			const { return m_Function_eax7_ecx0_EBX[30]; };
		bool HasAVX512_VL()			const { return m_Function_eax7_ecx0_EBX[31]; };

		// Function: [CPUID EAX=7, ECX=0] - ECX
		bool HasPREFETCHWT1()		const { return m_Function_eax7_ecx0_ECX[0]; };
		bool HasAVX512_VBMI()		const { return m_Function_eax7_ecx0_ECX[1]; };
		bool HasUMIP()				const { return m_Function_eax7_ecx0_ECX[2]; };
		bool HasPKU()				const { return m_Function_eax7_ecx0_ECX[3]; };
		bool HasOSPKE()				const { return m_Function_eax7_ecx0_ECX[4]; };
		bool HasWAITPKG()			const { return m_Function_eax7_ecx0_ECX[5]; };
		bool HasAVX512_VBMI_2()		const { return m_Function_eax7_ecx0_ECX[6]; };
		bool HasCET_SHSTK()			const { return m_Function_eax7_ecx0_ECX[7]; };
		bool HasGFNI()				const { return m_Function_eax7_ecx0_ECX[8]; };
		bool HasVAES()				const { return m_Function_eax7_ecx0_ECX[9]; };
		bool HasCLMUL()				const { return m_Function_eax7_ecx0_ECX[10]; };
		bool HasAVX512_VNNI()		const { return m_Function_eax7_ecx0_ECX[11]; };
		bool HasAVX512_BITALG()		const { return m_Function_eax7_ecx0_ECX[12]; };
		bool HasTME()				const { return m_Function_eax7_ecx0_ECX[13]; };
		bool HasAVX512_VPOPCNTDQ()	const { return m_Function_eax7_ecx0_ECX[14]; };
		//bool reserved()			const { return m_Function_eax7_ecx0_ECX[15]; };
		bool HasLA57()				const { return m_Function_eax7_ecx0_ECX[16]; };
		bool Mawau0()				const { return m_Function_eax7_ecx0_ECX[17]; };
		bool Mawau1()				const { return m_Function_eax7_ecx0_ECX[18]; };
		bool Mawau2()				const { return m_Function_eax7_ecx0_ECX[19]; };
		bool Mawau3()				const { return m_Function_eax7_ecx0_ECX[20]; };
		bool Mawau4()				const { return m_Function_eax7_ecx0_ECX[21]; };
		bool HasRDPID()				const { return m_Function_eax7_ecx0_ECX[22]; };
		bool HasAES_KL()			const { return m_Function_eax7_ecx0_ECX[23]; };
		bool HasBusLockDetect()		const { return m_Function_eax7_ecx0_ECX[24]; };
		bool HasCLDEMOTE()			const { return m_Function_eax7_ecx0_ECX[25]; };
		//bool reserved()			const { return m_Function_eax7_ecx0_ECX[26]; };
		bool HasMOVDIRI()			const { return m_Function_eax7_ecx0_ECX[27]; };
		bool HasMOVDIR64B()			const { return m_Function_eax7_ecx0_ECX[28]; };
		bool HasEMQCMD()			const { return m_Function_eax7_ecx0_ECX[29]; };
		bool HasSGX_LC()			const { return m_Function_eax7_ecx0_ECX[30]; };
		bool HasPKS()				const { return m_Function_eax7_ecx0_ECX[31]; };

		// Function: [CPUID EAX=7, ECX=0] - EDX
			//bool reserved()				const { return m_Function_eax7_ecx0_EDX[ 0]; };
		bool HasSGX_KEYS()				const { return m_Function_eax7_ecx0_EDX[1]; };
		bool HasAVX512_4vnniw()			const { return m_Function_eax7_ecx0_EDX[2]; };
		bool HasAVX512_4maps()			const { return m_Function_eax7_ecx0_EDX[3]; };
		bool HasFSRM()					const { return m_Function_eax7_ecx0_EDX[4]; };
		bool HasUINTR()					const { return m_Function_eax7_ecx0_EDX[5]; };
		//bool reserved()				const { return m_Function_eax7_ecx0_EDX[ 6]; };
		//bool reserved()				const { return m_Function_eax7_ecx0_EDX[ 7]; };
		bool HasAVX512_VP2intersect()	const { return m_Function_eax7_ecx0_EDX[8]; };
		bool HasSRDBS_CTRL()			const { return m_Function_eax7_ecx0_EDX[9]; };
		bool HasMC_CLEAR()				const { return m_Function_eax7_ecx0_EDX[10]; };
		bool TSXAlwaysAbort()			const { return m_Function_eax7_ecx0_EDX[11]; };
		//bool reserved()				const { return m_Function_eax7_ecx0_EDX[12]; };
		bool TSX_FORCE_ABORT_MSR()		const { return m_Function_eax7_ecx0_EDX[13]; };
		bool HasSERIALIZE()				const { return m_Function_eax7_ecx0_EDX[14]; };
		bool IsHybrid()					const { return m_Function_eax7_ecx0_EDX[15]; };
		bool HasTSKLDTRK()				const { return m_Function_eax7_ecx0_EDX[16]; };
		//bool reserved()				const { return m_Function_eax7_ecx0_EDX[17]; };
		bool HasPCONFIG()				const { return m_Function_eax7_ecx0_EDX[18]; };
		bool HasLBR()					const { return m_Function_eax7_ecx0_EDX[19]; };
		bool HasCET_IBT()				const { return m_Function_eax7_ecx0_EDX[20]; };
		//bool reserved()				const { return m_Function_eax7_ecx0_EDX[21]; };
		bool HasAMXbf16()				const { return m_Function_eax7_ecx0_EDX[22]; };
		bool HasAVX512_fp16()			const { return m_Function_eax7_ecx0_EDX[23]; };
		bool HasAMX_TILE()				const { return m_Function_eax7_ecx0_EDX[24]; };
		bool HasAMX_int8()				const { return m_Function_eax7_ecx0_EDX[25]; };
		bool HasIBRS_IBPB()				const { return m_Function_eax7_ecx0_EDX[26]; };
		bool HasSTIBP()					const { return m_Function_eax7_ecx0_EDX[27]; };
		bool HasL1D_FLUSH()				const { return m_Function_eax7_ecx0_EDX[28]; };
		bool IA32_ARCH_CAPABILITIES()	const { return m_Function_eax7_ecx0_EDX[29]; };
		bool IA32_CORE_CAPABILITIES()	const { return m_Function_eax7_ecx0_EDX[30]; };
		bool HasSSBD()					const { return m_Function_eax7_ecx0_EDX[31]; };


		// Function: [CPUID EAX=7, ECX=1] - EAX
			// bool reserved()				const { return m_Function_eax7_ecx1_EAX[ 0]; };
			// bool reserved()				const { return m_Function_eax7_ecx1_EAX[ 1]; };
			// bool reserved()				const { return m_Function_eax7_ecx1_EAX[ 2]; };
		bool HasRAO_INT()				const { return m_Function_eax7_ecx1_EAX[3]; };
		bool HasAVX_ANNI()				const { return m_Function_eax7_ecx1_EAX[4]; };
		bool HasAVX512_bf16()			const { return m_Function_eax7_ecx1_EAX[5]; };
		bool HasLASS()					const { return m_Function_eax7_ecx1_EAX[6]; };
		bool HasCMPCCXADD()				const { return m_Function_eax7_ecx1_EAX[7]; };
		bool HasARCHPERFMONEXT()		const { return m_Function_eax7_ecx1_EAX[8]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EAX[ 9]; };
		bool Fast_zero_rep_movsb()			const { return m_Function_eax7_ecx1_EAX[10]; };
		bool Fast_short_rep_stosb()			const { return m_Function_eax7_ecx1_EAX[11]; };
		bool Fast_short_rep_cmpsb_scasb()	const { return m_Function_eax7_ecx1_EAX[12]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EAX[13]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EAX[14]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EAX[15]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EAX[16]; };
		bool HasFRED()					const { return m_Function_eax7_ecx1_EAX[17]; };
		bool HasLKGS()					const { return m_Function_eax7_ecx1_EAX[18]; };
		bool HasWRMSRNS()				const { return m_Function_eax7_ecx1_EAX[19]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EAX[20]; };
		bool HasAMX_fp16()				const { return m_Function_eax7_ecx1_EAX[21]; };
		bool Has_hreset()				const { return m_Function_eax7_ecx1_EAX[22]; };
		bool HasAVX_ifma()				const { return m_Function_eax7_ecx1_EAX[23]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EAX[24]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EAX[25]; };
		bool HasLAM()					const { return m_Function_eax7_ecx1_EAX[26]; };
		bool Has_MSRLIST()				const { return m_Function_eax7_ecx1_EAX[27]; };
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
		bool HasAVX_VNN_int8()			const { return m_Function_eax7_ecx1_EDX[4]; };
		bool HasAVX_ne_convert()		const { return m_Function_eax7_ecx1_EDX[5]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EDX[ 6]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EDX[ 7]; };
		bool HasAMX_COMPLEX()			const { return m_Function_eax7_ecx1_EDX[8]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EDX[ 9]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EDX[10]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EDX[11]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EDX[12]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EDX[13]; };
		bool HasPREFETCHI()				const { return m_Function_eax7_ecx1_EDX[14]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EDX[15]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EDX[16]; };
		// bool reserved()				const { return m_Function_eax7_ecx1_EDX[17]; };
		bool HasCETSSS()				const { return m_Function_eax7_ecx1_EDX[18]; };
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
		bool HasPFSD()					const { return m_Function_eax7_ecx2_EDX[0]; };
		bool HasIPRED_DIS()				const { return m_Function_eax7_ecx2_EDX[1]; };
		bool HasRRSBA()					const { return m_Function_eax7_ecx2_EDX[2]; };
		bool HasDPPD_U()				const { return m_Function_eax7_ecx2_EDX[3]; };
		bool HasBHI_DIS_S()				const { return m_Function_eax7_ecx2_EDX[4]; };
		bool HasMCDT_NO()				const { return m_Function_eax7_ecx2_EDX[5]; };

		// Function: [CPUID EAX=80000001h] - EDX
	public:
		bool HasFPU_AMD()				const { return m_Function_81h_EDX[0]; };
		bool HasVME_AMD()				const { return m_Function_81h_EDX[1]; };
		bool HasDebugExtensions_AMD()	const { return m_Function_81h_EDX[2]; };
		bool HasPSE_AMD()				const { return m_Function_81h_EDX[3]; };
		bool HasRDTSC_AMD()				const { return m_Function_81h_EDX[4]; };
		bool HasMSR_AMD()				const { return m_Function_81h_EDX[5]; };
		bool HasPAE_AMD()				const { return m_Function_81h_EDX[6]; };
		bool HasMCE_AMD()				const { return m_Function_81h_EDX[7]; };
		bool HasCX8_AMD()				const { return m_Function_81h_EDX[8]; };
		bool HasAPIC_AMD()				const { return m_Function_81h_EDX[9]; };
		bool syscall_K6()				const { return m_Function_81h_EDX[10]; };
		bool syscall()					const { return m_Function_81h_EDX[11]; };
		bool HasMTRR_AMD()				const { return m_Function_81h_EDX[12]; };
		bool HasPGE_AMD()				const { return m_Function_81h_EDX[13]; };
		bool HasMCA_AMD()				const { return m_Function_81h_EDX[14]; };
		bool HasCMOV_AMD()				const { return m_Function_81h_EDX[15]; };
		bool HasPAT_AMD()				const { return m_Function_81h_EDX[16]; };
		bool HasPSE36_AMD()				const { return m_Function_81h_EDX[17]; };
		//bool reserved()				const { return m_Function_81h_EDX[18]; };
		bool HasECC()					const { return m_Function_81h_EDX[19]; };
		bool HasNX_AMD()				const { return m_Function_81h_EDX[20]; };
		//bool reserved()				const { return m_Function_81h_EDX[21]; };
		bool HasMMEXT()					const { return m_Function_81h_EDX[22]; };
		bool HasMMX_AMD()				const { return m_Function_81h_EDX[23]; };
		bool HasFXSR_AMD()				const { return m_Function_81h_EDX[24]; };
		bool HasFXSR_OPT()				const { return m_Function_81h_EDX[25]; };
		bool HasPDPE1GB()				const { return m_Function_81h_EDX[26]; };
		bool HasRDTSCP()				const { return m_Function_81h_EDX[27]; };
		//bool reserved()				const { return m_Function_81h_EDX[28]; };
		bool HasLM()					const { return m_Function_81h_EDX[29]; };
		bool Has3DNOWEXT()				const { return m_Function_81h_EDX[30]; };
		bool Has3DNOW()					const { return m_Function_81h_EDX[31]; };

		// Function: [CPUID EAX=80000001h] - ECX
	public:
		bool HasLAHF_LM()					const { return m_Function_81h_ECX[0]; };
		bool HasCMP_LEGACY()				const { return m_Function_81h_ECX[1]; };
		bool HasSVM()						const { return m_Function_81h_ECX[2]; };
		bool HasEXTAPIC()					const { return m_Function_81h_ECX[3]; };
		bool HasCR8_LEGACY()				const { return m_Function_81h_ECX[4]; };
		bool HasABM()						const { return m_Function_81h_ECX[5]; };
		bool HasSSE4A()						const { return m_Function_81h_ECX[6]; };
		bool HassMISALIGNSSE()				const { return m_Function_81h_ECX[7]; };
		bool Has3DNOWPREFETCH()				const { return m_Function_81h_ECX[8]; };
		bool HasOSVW()						const { return m_Function_81h_ECX[9]; };
		bool HasIBS()						const { return m_Function_81h_ECX[10]; };
		bool HasXOP()						const { return m_Function_81h_ECX[11]; };
		bool HasSKINIT()					const { return m_Function_81h_ECX[12]; };
		bool HasWDT()						const { return m_Function_81h_ECX[13]; };
		// bool reserved()					const { return m_Function_81h_ECX[14]; };
		bool HasLWP()						const { return m_Function_81h_ECX[15]; };
		bool HasFMA4()						const { return m_Function_81h_ECX[16]; };
		bool HasTCE()						const { return m_Function_81h_ECX[17]; };
		// bool reserved()					const { return m_Function_81h_ECX[18]; };
		bool HasNODEID_MSR()				const { return m_Function_81h_ECX[19]; };
		// bool reserved()					const { return m_Function_81h_ECX[20]; };
		bool HasTBM()						const { return m_Function_81h_ECX[21]; };
		bool HasTOPOEXT()					const { return m_Function_81h_ECX[22]; };
		bool HasPERFCTR_CORE()				const { return m_Function_81h_ECX[23]; };
		bool HasPERFCTR_NB()				const { return m_Function_81h_ECX[24]; };
		//bool reserved()					const { return m_Function_81h_ECX[25]; };
		bool HasDBX()						const { return m_Function_81h_ECX[26]; };
		bool HasPERFTSC()					const { return m_Function_81h_ECX[27]; };
		bool HasPCX_L2I()					const { return m_Function_81h_ECX[28]; };
		bool HasMONITORX()					const { return m_Function_81h_ECX[29]; };
		bool HasADDR_MASK_EXT()				const { return m_Function_81h_ECX[30]; };
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
	void DisplayCPUInformation();
}