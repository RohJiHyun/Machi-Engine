#include "Device.h"
#include <iostream>
#include "dxhelper.h"

#include "native_format_converter.h"
using namespace Machi::NativeGraphics;
bool Device::init_device(bool debug_mode)
{
	UINT dxgiFactoryFlags = 0;
	ComPtr<IDXGIFactory4> factory = nullptr;

#if defined(_DEBUG)
	// Enable the debug layer (requires the Graphics Tools "optional feature").
	// NOTE: Enabling the debug layer after device creation will invalidate the active device.
	{
		ComPtr<ID3D12Debug> debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
		{
			debugController->EnableDebugLayer();

			// Enable additional debug layers.
			dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
		}
	}
#else
	if (debug_mode) {
		ComPtr<ID3D12Debug> debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
		{
			debugController->EnableDebugLayer();

			// Enable additional debug layers.
			dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
		}
	}
#endif

	try {
		ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory)));
	}
	catch (HrException& e) {
		std::cerr << e.what() << std::endl;
		return false;
	}



	try {

	ComPtr<IDXGIAdapter1> hardwareAdapter;
	//GetHardwareAdapter(factory.Get(), &hardwareAdapter, true);
	GetHardwareAdapter(factory.Get(), &hardwareAdapter, false);

	ThrowIfFailed(D3D12CreateDevice(
		hardwareAdapter.Get(),
		D3D_FEATURE_LEVEL_11_0,
		IID_PPV_ARGS(&m_device)
	));
	}
	catch (HrException& e) {
		std::cerr << e.what() << std::endl;
		return false;
	}



	return true;
}

HRESULT 
Device::create_fence(MUINT init_value, Machi::Graphics::FenceFlags flag, ComPtr<ID3D12Fence>& fence_com_ptr) {
	//Machi::Logger::MLogger::get_instance().debug(MACHI_DEFAULT_CONSOLE_LOGGER_NAME, &fence_com_ptr);
	//Machi::Logger::MLogger::get_instance().debug(MACHI_DEFAULT_CONSOLE_LOGGER_NAME, fence_com_ptr);
	//Machi::Logger::MLogger::get_instance().debug(MACHI_DEFAULT_CONSOLE_LOGGER_NAME, fence_com_ptr.Get());
	//Machi::Logger::MLogger::get_instance().debug(MACHI_DEFAULT_CONSOLE_LOGGER_NAME, fence_com_ptr.GetAddressOf());
	HRESULT res = m_device->CreateFence(init_value, fence_flag_convert(flag), IID_PPV_ARGS(&fence_com_ptr));
	return res;
}
HRESULT 
Device::create_command_queue(D3D12_COMMAND_QUEUE_DESC* desc, ComPtr<ID3D12CommandQueue>& command_queue) {
	return m_device->CreateCommandQueue(desc, IID_PPV_ARGS(&command_queue));
}

HRESULT Machi::NativeGraphics::Device::create_root_signature(MUINT node_mask, const void* buf_ptr, MUINT buf_size, ComPtr<ID3D12RootSignature> root_sig)
{


	return m_device->CreateRootSignature(node_mask, buf_ptr, buf_size, IID_PPV_ARGS(&root_sig));
	
}

HRESULT Machi::NativeGraphics::Device::check_feature_support(D3D12_FEATURE Feature, void* pFeatureSupportData, UINT FeatureSupportDataSize) {

	return m_device->CheckFeatureSupport(Feature, pFeatureSupportData, FeatureSupportDataSize);
}

HRESULT Machi::NativeGraphics::Device::create_graphics_pipelinestate(const D3D12_GRAPHICS_PIPELINE_STATE_DESC* p_desc, ComPtr<ID3D12PipelineState> m_pipeline_object) {


	return m_device->CreateGraphicsPipelineState(p_desc, IID_PPV_ARGS(&m_pipeline_object));

}