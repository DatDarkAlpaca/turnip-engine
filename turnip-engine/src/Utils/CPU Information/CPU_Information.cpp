#include "pch.h"
#include "CPU_Information.h"

struct CPU_ID
{
public:
	explicit CPU_ID(U32 eax, U32 ecx)
	{
		// Todo: write a cpuid function for linux/macos
#ifdef TUR_PLATFORM_WINDOWS
		__cpuidex((int*)registers, eax, ecx);
#endif
	}

	explicit CPU_ID(U32 eax)
	{
#ifdef TUR_PLATFORM_WINDOWS
		__cpuid((int*)registers, eax);
#endif
	}

public:
	const U32& eax() const { return registers[0]; }

	const U32& ebx() const { return registers[1]; }

	const U32& ecx() const { return registers[2]; }

	const U32& edx() const { return registers[3]; }

public:
	U32 registers[4];
};

namespace tur
{
	CPU_Information::CPU_Information()
	{
		prepareVendor();

		prepareFunctions();
	}

	void CPU_Information::prepareVendor()
	{
		CPU_ID cpuID(0x0);

		m_Vendor += std::string((const char*)&cpuID.ebx(), 4);
		m_Vendor += std::string((const char*)&cpuID.edx(), 4);
		m_Vendor += std::string((const char*)&cpuID.ecx(), 4);

		const std::unordered_map<std::string, std::string> vendorMap = {
			{ "AMDisbetter!", "AMD" },
			{ "AuthenticAMD!", "AMD" },
			{ "GenuineIntel", "Intel" }
		};

		if (vendorMap.find(m_Vendor) != vendorMap.end())
		{
			m_Vendor = vendorMap.at(m_Vendor);
			return;
		}

		m_Vendor = "Unknown Vendor";
		m_IsAMD = m_Vendor == "AMD";
		m_IsIntel = m_Vendor == "Intel";
	}

	std::string CPU_Information::brand() const
	{
		char str[sizeof(U32) * 13];

		CPU_ID cpuID_Part0 = CPU_ID(0x80000000);
		if (cpuID_Part0.eax() < 0x80000004)
			return "Invalid CPU brand name";

		cpuID_Part0 = CPU_ID(0x80000002);
		CPU_ID cpuID_Part1 = CPU_ID(0x80000003);
		CPU_ID cpuID_Part2 = CPU_ID(0x80000004);

		memcpy(str, (U32*)cpuID_Part0.registers, sizeof(cpuID_Part0.registers));
		memcpy(str + sizeof(cpuID_Part0.registers), (U32*)cpuID_Part1.registers, sizeof(cpuID_Part1.registers));
		memcpy(str + sizeof(cpuID_Part0.registers) + sizeof(cpuID_Part1.registers), (U32*)cpuID_Part2.registers, sizeof(cpuID_Part2.registers));

		str[sizeof(str) - 1] = '\0';
		return std::string(str);
	}

	void CPU_Information::prepareFunctions()
	{
		CPU_ID cpuID(0x1);
		m_Function_eax1_ECX = cpuID.ecx();
		m_Function_eax1_EDX = cpuID.edx();

		cpuID = CPU_ID(0x7, 0x0);
		m_Function_eax7_ecx0_EBX = cpuID.ebx();
		m_Function_eax7_ecx0_ECX = cpuID.ecx();
		m_Function_eax7_ecx0_EDX = cpuID.edx();

		cpuID = CPU_ID(0x7, 0x1);
		m_Function_eax7_ecx1_EAX = cpuID.eax();
		m_Function_eax7_ecx1_EBX = cpuID.ebx();
		m_Function_eax7_ecx1_EDX = cpuID.edx();

		cpuID = CPU_ID(0x7, 0x2);
		m_Function_eax7_ecx2_EDX = cpuID.edx();

		cpuID = CPU_ID(0x80000001);
		m_Function_81h_ECX = cpuID.ecx();
		m_Function_81h_EDX = cpuID.edx();
	}
}

namespace tur
{
	void display_cpu_information()
	{
		CPU_Information information;

		auto supported = [](bool supported) -> const char* {
			return supported ? "Yes" : "No";
		};

		TUR_CORE_DEBUG("{} {}", information.vendor(), information.brand());
		TUR_CORE_DEBUG("Instructions:");
		TUR_CORE_DEBUG("* 3DNOW:     {}", supported(information.has3DNOW()));
		TUR_CORE_DEBUG("* 3DNOWEXT:  {}", supported(information.has3DNOWEXT()));
		TUR_CORE_DEBUG("* SSE:       {}", supported(information.hasSSE()));
		TUR_CORE_DEBUG("* SSE2:      {}", supported(information.hasSSE2()));
		TUR_CORE_DEBUG("* SSE3:      {}", supported(information.hasSSE3()));
		TUR_CORE_DEBUG("* SSSE3:     {}", supported(information.hasS_SSE3()));
		TUR_CORE_DEBUG("* SSE4A:     {}", supported(information.hasSSE4A()));
		TUR_CORE_DEBUG("* SSE4.1:    {}", supported(information.hasSSE4_1()));
		TUR_CORE_DEBUG("* SSE4.2:    {}", supported(information.hasSSE4_2()));
		TUR_CORE_DEBUG("* MMX:       {}", supported(information.hasMMX()));
		TUR_CORE_DEBUG("* MMXEXT:    {}", supported(information.hasMMEXT()));
		TUR_CORE_DEBUG("* CMOV:      {}", supported(information.hasCMOV()));
		TUR_CORE_DEBUG("* RDTSC:     {}", supported(information.hasRDTSC()));
		TUR_CORE_DEBUG("* FPU:       {}", supported(information.hasFPU()));
		TUR_CORE_DEBUG("* FPUx87:    {}", supported(information.hasFPUx87()));
		TUR_CORE_DEBUG("* HTT:       {}", supported(information.hasHTT()));
	}
}