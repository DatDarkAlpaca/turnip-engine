#include "pch.hpp"
#include "CPUInfo.hpp"
#include "Common.hpp"

namespace tur
{
	struct CPU_ID
	{
	public:
		explicit CPU_ID(uint32_t eax, uint32_t ecx)
		{
			// Todo: write a cpuid function for linux/macos
#ifdef TUR_PLATFORM_WIN32
			__cpuidex(reinterpret_cast<int*>(registers), eax, ecx);
#endif
		}

		explicit CPU_ID(uint32_t eax)
		{
#ifdef TUR_PLATFORM_WIN32
			__cpuid(reinterpret_cast<int*>(registers), eax);
#endif
		}

	public:
		const uint32_t& eax() const { return registers[0]; }

		const uint32_t& ebx() const { return registers[1]; }

		const uint32_t& ecx() const { return registers[2]; }

		const uint32_t& edx() const { return registers[3]; }

	public:
		uint32_t registers[4] = { 0, 0, 0, 0 };
	};
}

namespace tur
{
	CPU_Information::CPU_Information()
	{
		PrepareVendor();

		PrepareFunctions();
	}

	void CPU_Information::PrepareVendor()
	{
		CPU_ID cpuID(0x0);

		m_Vendor += std::string(reinterpret_cast<const char*>(&cpuID.ebx()), 4);
		m_Vendor += std::string(reinterpret_cast<const char*>(&cpuID.edx()), 4);
		m_Vendor += std::string(reinterpret_cast<const char*>(&cpuID.ecx()), 4);

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

	std::string CPU_Information::Brand() const
	{
		char str[sizeof(uint32_t) * 13];

		CPU_ID cpuID_Part0 = CPU_ID(0x80000000);
		if (cpuID_Part0.eax() < 0x80000004)
			return "Invalid CPU brand name";

		cpuID_Part0 = CPU_ID(0x80000002);
		CPU_ID cpuID_Part1 = CPU_ID(0x80000003);
		CPU_ID cpuID_Part2 = CPU_ID(0x80000004);

		memcpy(
			str, 
			reinterpret_cast<uint32_t*>(cpuID_Part0.registers), 
			sizeof(cpuID_Part0.registers)
		);

		memcpy(
			str + sizeof(cpuID_Part0.registers), 
			reinterpret_cast<uint32_t*>(cpuID_Part1.registers),
			sizeof(cpuID_Part1.registers)
		);

		memcpy(
			str + sizeof(cpuID_Part0.registers) + sizeof(cpuID_Part1.registers), 
			reinterpret_cast<uint32_t*>(cpuID_Part2.registers),
			sizeof(cpuID_Part2.registers)
		);

		str[sizeof(str) - 1] = '\0';
		return std::string(str);
	}

	void CPU_Information::PrepareFunctions()
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
	void DisplayCPUInformation()
	{
		CPU_Information information;

		auto supported = [](bool supported) -> const char* {
			return supported ? "Yes" : "No";
		};

		TUR_LOG_DEBUG("{} {}", information.Vendor(), information.Brand());
		TUR_LOG_DEBUG("Instructions:");
		TUR_LOG_DEBUG(" * 3DNOW:     {}", supported(information.Has3DNOW()));
		TUR_LOG_DEBUG(" * 3DNOWEXT:  {}", supported(information.Has3DNOWEXT()));
		TUR_LOG_DEBUG(" * SSE:       {}", supported(information.HasSSE()));
		TUR_LOG_DEBUG(" * SSE2:      {}", supported(information.HasSSE2()));
		TUR_LOG_DEBUG(" * SSE3:      {}", supported(information.HasSSE3()));
		TUR_LOG_DEBUG(" * SSSE3:     {}", supported(information.HasS_SSE3()));
		TUR_LOG_DEBUG(" * SSE4A:     {}", supported(information.HasSSE4A()));
		TUR_LOG_DEBUG(" * SSE4.1:    {}", supported(information.HasSSE4_1()));
		TUR_LOG_DEBUG(" * SSE4.2:    {}", supported(information.HasSSE4_2()));
		TUR_LOG_DEBUG(" * MMX:       {}", supported(information.HasMMX()));
		TUR_LOG_DEBUG(" * MMXEXT:    {}", supported(information.HasMMEXT()));
		TUR_LOG_DEBUG(" * CMOV:      {}", supported(information.HasCMOV()));
		TUR_LOG_DEBUG(" * RDTSC:     {}", supported(information.HasRDTSC()));
		TUR_LOG_DEBUG(" * FPU:       {}", supported(information.HasFPU()));
		TUR_LOG_DEBUG(" * FPUx87:    {}", supported(information.HasFPUx87()));
		TUR_LOG_DEBUG(" * HTT:       {}", supported(information.HasHTT()));
	}
}