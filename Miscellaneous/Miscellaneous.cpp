
#if !defined(OnlyOne) || defined(NumberOfCores)
#include <windows.h>
#include <assert.h>
#include <stdint.h>

int32_t NumberOfCores()
{
	static int32_t CoreCount = 0;
	if (CoreCount == 0)
	{
		// Get only physical cores
		PSYSTEM_LOGICAL_PROCESSOR_INFORMATION InfoBuffer = NULL;
		::DWORD BufferSize = 0;

		// Get the size of the buffer to hold processor information.
		::BOOL Result = GetLogicalProcessorInformation(InfoBuffer, &BufferSize);
		assert(!Result && GetLastError() == ERROR_INSUFFICIENT_BUFFER);
		assert(BufferSize > 0);

		// Allocate the buffer to hold the processor info.
		InfoBuffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)malloc(BufferSize);
		assert(InfoBuffer);

		// Get the actual information.
		Result = GetLogicalProcessorInformation(InfoBuffer, &BufferSize);
		assert(Result);

		// Count physical cores
		const int32_t InfoCount = (int32_t)(BufferSize / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION));
		for (int32_t Index = 0; Index < InfoCount; ++Index)
		{
			SYSTEM_LOGICAL_PROCESSOR_INFORMATION* Info = &InfoBuffer[Index];
			if (Info->Relationship == RelationProcessorCore)
			{
				CoreCount++;
			}
		}
		free(InfoBuffer);
	}
	return CoreCount;
}

#endif

#include <stdio.h>

int main()
{
	printf("the number of core is %d", NumberOfCores());
}