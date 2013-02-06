#include "gyRenderer.h"
#if defined(__GY_OS_ENABLED_WINSTORE__) || defined(__GY_OS_ENABLED_WINPHONE__)
// For MS-Windows RT
using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;
namespace gy
{
	namespace r3
	{
		bool isValid(renderer_t rid);
		bool isValid();
		renderer_t getCurrentID();

		// DirectXHelpers
		// Method to convert a length in device-independent pixels (DIPs) to a length in physical pixels.
		float DX_ConvertDipsToPixels(float dips)
		{
			static const float dipsPerInch = 96.0f;
			return floor(dips * DisplayProperties::LogicalDpi / dipsPerInch + 0.5f); // Round to nearest integer.
		}

		void DX_ThrowIfFailed(HRESULT hr)
		{
			if (FAILED(hr))
			{
				// Set a breakpoint on this line to catch Win32 API errors.
				throw Platform::Exception::CreateException(hr);
			}
		}

		// Function that reads from a binary file asynchronously.
		inline Concurrency::task<Platform::Array<byte>^> ReadDataAsync(Platform::String^ filename)
		{
			using namespace Windows::Storage;
			using namespace Concurrency;

			auto folder = Windows::ApplicationModel::Package::Current->InstalledLocation;

			return create_task(folder->GetFileAsync(filename)).then([] (StorageFile^ file) 
			{
				return file->OpenReadAsync();
			}).then([] (Streams::IRandomAccessStreamWithContentType^ stream)
			{
				unsigned int bufferSize = static_cast<unsigned int>(stream->Size);
				auto fileBuffer = ref new Streams::Buffer(bufferSize);
				return stream->ReadAsync(fileBuffer, bufferSize, Streams::InputStreamOptions::None);
			}).then([] (Streams::IBuffer^ fileBuffer) -> Platform::Array<byte>^ 
			{
				auto fileData = ref new Platform::Array<byte>(fileBuffer->Length);
				Streams::DataReader::FromBuffer(fileBuffer)->ReadBytes(fileData);
				return fileData;
			});
		}

		// X11연관 함수들
#define MAX_RENDERER_CONTEXT 1
		_renderer_object_t renderer_objects[MAX_RENDERER_CONTEXT] = {{NullPtr, NullPtr, NullPtr, NullPtr, NullPtr},};
		size_t current_renderer_id = GY_INVALID_RENDERER_ID;

		renderer_t getAllocableID()
		{
			for (size_t i = 0; i < MAX_RENDERER_CONTEXT; ++i)
			{
				if (isValid(i) == true)
					continue;

				return i;
			}

			return GY_INVALID_RENDERER_ID;
		}

		uint32 getMaxAllocableObjectCount()
		{
			return MAX_RENDERER_CONTEXT;
		}

		bool isValid()
		{
			const renderer_t rid = getCurrentID();
			return isValid(rid);
		}

		bool isValid(renderer_t rid)
		{
			if (rid == GY_INVALID_RENDERER_ID)
				return false;

			int rendererAPIType = getRendererAPIType();
			renderer_object_t robj = &renderer_objects[rid];
			bool ret = rendererAPIType != GY_RENDERER_UNKNOWN 
				&& rendererAPIType < GY_RENDERER_TYPE_SIZE 
				&& robj != NullPtr
				&& robj->Window != NULL
				&& robj->Device != NULL
				&& robj->Context != NULL
				&& robj->SwapChain != NULL
				&& robj->RenderTargetView != NULL
				&& robj->DepthStencilView != NULL;
				//&& robj->RendererContext != wglGetCurrentContext();

			return ret;
		}

		int setCurrentRendererID(renderer_t rid)
		{
			current_renderer_id = rid;

			return Success;
		}

		renderer_t getCurrentID()
		{
			return current_renderer_id;
		}

		renderer_object_t getRendererObject(renderer_t rid)
		{
			if (isValid(rid) == false)
				return NullPtr;

			return &renderer_objects[rid];
		}

		void createDeviceResources(renderer_object_t robj)
		{
			// This flag adds support for surfaces with a different color channel ordering
			// than the API default. It is required for compatibility with Direct2D.
			UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
			// If the project is in a debug build, enable debugging via SDK Layers with this flag.
			creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

			// This array defines the set of DirectX hardware feature levels this app will support.
			// Note the ordering should be preserved.
			// Don't forget to declare your application's minimum required feature level in its
			// description.  All applications are assumed to support 9.1 unless otherwise stated.
			D3D_FEATURE_LEVEL featureLevels[] = 
			{
				D3D_FEATURE_LEVEL_11_1,
				D3D_FEATURE_LEVEL_11_0,
				D3D_FEATURE_LEVEL_10_1,
				D3D_FEATURE_LEVEL_10_0,
				D3D_FEATURE_LEVEL_9_3,
				D3D_FEATURE_LEVEL_9_2,
				D3D_FEATURE_LEVEL_9_1
			};

			// Create the Direct3D 11 API device object and a corresponding context.
			Microsoft::WRL::ComPtr<ID3D11Device> device;
			Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
			DX_ThrowIfFailed(
				D3D11CreateDevice(
				nullptr, // Specify nullptr to use the default adapter.
				D3D_DRIVER_TYPE_HARDWARE,
				nullptr,
				creationFlags, // Set set debug and Direct2D compatibility flags.
				featureLevels, // List of feature levels this app can support.
				ARRAYSIZE(featureLevels),
				D3D11_SDK_VERSION, // Always set this to D3D11_SDK_VERSION for Windows Store apps.
				&device, // Returns the Direct3D device created.
				&(robj->FeatureLevel), // Returns feature level of device created.
				&context // Returns the device immediate context.
				)
				);

			// Get the Direct3D 11.1 API device and context interfaces.
			DX_ThrowIfFailed(device.As(&(robj->Device)));
			DX_ThrowIfFailed(context.As(&(robj->Context)));
		}

		void createWindowSizeDependentResources(renderer_object_t robj)
		{
			// Store the window bounds so the next time we get a SizeChanged event we can
			// avoid rebuilding everything if the size is identical.
			robj->WindowBounds = robj->Window->Bounds;

			// Calculate the necessary swap chain and render target size in pixels.
			float windowWidth = DX_ConvertDipsToPixels(robj->WindowBounds.Width);// * 0.5f);
			float windowHeight = DX_ConvertDipsToPixels(robj->WindowBounds.Height);// * 0.5f);

			// The width and height of the swap chain must be based on the window's
			// landscape-oriented width and height. If the window is in a portrait
			// orientation, the dimensions must be reversed.
			robj->Orientation = DisplayProperties::CurrentOrientation;
			bool swapDimensions =
				robj->Orientation == DisplayOrientations::Portrait ||
				robj->Orientation == DisplayOrientations::PortraitFlipped;
			robj->RenderTargetSize.Width = swapDimensions ? windowHeight : windowWidth;
			robj->RenderTargetSize.Height = swapDimensions ? windowWidth : windowHeight;

			if(robj->SwapChain != nullptr)
			{
				// If the swap chain already exists, resize it.
				DX_ThrowIfFailed(
					robj->SwapChain->ResizeBuffers(
					2, // Double-buffered swap chain.
					static_cast<UINT>(robj->RenderTargetSize.Width),
					static_cast<UINT>(robj->RenderTargetSize.Height),
					DXGI_FORMAT_B8G8R8A8_UNORM,
					0
					)
					);
			}
			else
			{
				// Otherwise, create a new one using the same adapter as the existing Direct3D device.
				DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {0};
				swapChainDesc.Width = static_cast<UINT>(robj->RenderTargetSize.Width); // Match the size of the window.
				swapChainDesc.Height = static_cast<UINT>(robj->RenderTargetSize.Height);
				swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // This is the most common swap chain format.
				swapChainDesc.Stereo = false;
				swapChainDesc.SampleDesc.Count = 1; // Don't use multi-sampling.
				swapChainDesc.SampleDesc.Quality = 0;
				swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
				swapChainDesc.BufferCount = 2; // Use double-buffering to minimize latency.
				swapChainDesc.Scaling = DXGI_SCALING_NONE; // DXGI_SCALING_STRETCH;
				swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // All Windows Store apps must use this SwapEffect.
				swapChainDesc.Flags = 0;

				Microsoft::WRL::ComPtr<IDXGIDevice1> dxgiDevice;
				DX_ThrowIfFailed(
					robj->Device.As(&dxgiDevice)
					);

				Microsoft::WRL::ComPtr<IDXGIAdapter> dxgiAdapter;
				DX_ThrowIfFailed(
					dxgiDevice->GetAdapter(&dxgiAdapter)
					);

				Microsoft::WRL::ComPtr<IDXGIFactory2> dxgiFactory;
				DX_ThrowIfFailed(
					dxgiAdapter->GetParent(
					__uuidof(IDXGIFactory2), 
					&dxgiFactory
					)
					);

				Windows::UI::Core::CoreWindow^ window = robj->Window.Get();
				DX_ThrowIfFailed(
					dxgiFactory->CreateSwapChainForCoreWindow(
					robj->Device.Get(),
					reinterpret_cast<IUnknown*>(window),
					&swapChainDesc,
					nullptr, // Allow on all displays.
					&robj->SwapChain
					)
					);

				// Ensure that DXGI does not queue more than one frame at a time. This both reduces latency and
				// ensures that the application will only render after each VSync, minimizing power consumption.
				DX_ThrowIfFailed(
					dxgiDevice->SetMaximumFrameLatency(1)
					);
			}

			// Set the proper orientation for the swap chain, and generate the
			// 3D matrix transformation for rendering to the rotated swap chain.
			DXGI_MODE_ROTATION rotation = DXGI_MODE_ROTATION_UNSPECIFIED;
			switch (robj->Orientation)
			{
			case DisplayOrientations::Landscape:
				rotation = DXGI_MODE_ROTATION_IDENTITY;
				robj->OrientationTransform3D = XMFLOAT4X4( // 0-degree Z-rotation
					1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f
					);
				break;

			case DisplayOrientations::Portrait:
				rotation = DXGI_MODE_ROTATION_ROTATE270;
				robj->OrientationTransform3D = XMFLOAT4X4( // 90-degree Z-rotation
					0.0f, 1.0f, 0.0f, 0.0f,
					-1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f
					);
				break;

			case DisplayOrientations::LandscapeFlipped:
				rotation = DXGI_MODE_ROTATION_ROTATE180;
				robj->OrientationTransform3D = XMFLOAT4X4( // 180-degree Z-rotation
					-1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, -1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f
					);
				break;

			case DisplayOrientations::PortraitFlipped:
				rotation = DXGI_MODE_ROTATION_ROTATE90;
				robj->OrientationTransform3D = XMFLOAT4X4( // 270-degree Z-rotation
					0.0f, -1.0f, 0.0f, 0.0f,
					1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f
					);
				break;

			default:
				throw ref new Platform::FailureException();
			}

			DX_ThrowIfFailed(robj->SwapChain->SetRotation(rotation));

			// Create a render target view of the swap chain back buffer.
			Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
			DX_ThrowIfFailed(
				robj->SwapChain->GetBuffer(
				0,
				__uuidof(ID3D11Texture2D),
				&backBuffer
				)
				);

			DX_ThrowIfFailed(
				robj->Device->CreateRenderTargetView(
				backBuffer.Get(),
				nullptr,
				&robj->RenderTargetView
				)
				);

			// Create a depth stencil view.
			CD3D11_TEXTURE2D_DESC depthStencilDesc(
				DXGI_FORMAT_D24_UNORM_S8_UINT, 
				static_cast<UINT>(robj->RenderTargetSize.Width),
				static_cast<UINT>(robj->RenderTargetSize.Height),
				1,
				1,
				D3D11_BIND_DEPTH_STENCIL
				);

			Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencil;
			DX_ThrowIfFailed(
				robj->Device->CreateTexture2D(
				&depthStencilDesc,
				nullptr,
				&depthStencil
				)
				);

			CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
			DX_ThrowIfFailed(
				robj->Device->CreateDepthStencilView(
				depthStencil.Get(),
				&depthStencilViewDesc,
				&robj->DepthStencilView
				)
				);

			// Set the rendering viewport to target the entire window.
			CD3D11_VIEWPORT viewport(
				0.0f,
				0.0f,
				robj->RenderTargetSize.Width,
				robj->RenderTargetSize.Height
				);

			robj->Context->RSSetViewports(1, &viewport);
		}

		void updateForWindowSizeChange(renderer_object_t robj)
		{
			if (robj->Window->Bounds.Width  != robj->WindowBounds.Width ||
				robj->Window->Bounds.Height != robj->WindowBounds.Height ||
				robj->Orientation != DisplayProperties::CurrentOrientation)
			{
				ID3D11RenderTargetView* nullViews[] = {nullptr};
				robj->Context->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
				robj->RenderTargetView = nullptr;
				robj->DepthStencilView = nullptr;
				robj->Context->Flush();
				createWindowSizeDependentResources(robj);
			}
		}

		void handleDeviceLost()
		{
			if (isValid() == false)
				return;

			const renderer_t rid = getCurrentID();
			renderer_object_t robj = &renderer_objects[rid];

			createDeviceResources(robj);
			updateForWindowSizeChange(robj);
		}

		// OS별 특징이 있는 공통 함수들
		renderer_t createRenderer(void *vid, size_t witdh, size_t height)
		{
			const renderer_t allocableID = getAllocableID();
			if (allocableID == GY_INVALID_RENDERER_ID)
				return GY_INVALID_RENDERER_ID;

			renderer_object_t robj = &renderer_objects[allocableID];
			robj->Window = reinterpret_cast<Windows::UI::Core::CoreWindow ^>(vid);

			createDeviceResources(robj);
			createWindowSizeDependentResources(robj);
			setCurrentRendererID(allocableID);

			return Success;
		}

		renderer_t createRenderer(const _renderer_object_t &rawRendererContext)
		{
			return Success;
		}

		int destroyRenderer(renderer_t rid)
		{
			return Success;
		}

		uint32 clear_color = 0xFF0000FF;
		uint32 getClearColor()
		{
			return clear_color;
		}

        int setClearColor(const uint32 &color)
		{
			clear_color = color;

			return Success;
		}

		int clear()
		{
			if (isValid() == false)
				return Failed;

			const renderer_t rid = getCurrentID();
			renderer_object_t robj = &renderer_objects[rid];

			c4f cf((float)((clear_color << 8) >> 24) / 255.0f
				, (float)((clear_color << 16) >> 24) / 255.0f
				, (float)((clear_color << 24) >> 24) / 255.0f
				, (float)(clear_color >> 24) / 255.0f);
			const float clearColor[] = {cf.r, cf.g, cf.b, cf.a};
			robj->Context->ClearRenderTargetView(
				robj->RenderTargetView.Get(),
				clearColor
				);

			robj->Context->ClearDepthStencilView(
				robj->DepthStencilView.Get(),
				D3D11_CLEAR_DEPTH,
				1.0f,
				0
				);

			// Only draw the cube once it is loaded (loading is asynchronous).
			//if (!m_loadingComplete)
			//{
			//	return;
			//}

			robj->Context->OMSetRenderTargets(
				1,
				robj->RenderTargetView.GetAddressOf(),
				robj->DepthStencilView.Get()
				);

			return Success;
		}

		int begin()
		{
			return Success;
		}

		int end()
		{
			return Success;
		}

		int flush()
		{
			if (isValid() == false)
				return Failed;

			const renderer_t rid = getCurrentID();
			renderer_object_t robj = &renderer_objects[rid];

			// The application may optionally specify "dirty" or "scroll"
			// rects to improve efficiency in certain scenarios.
			DXGI_PRESENT_PARAMETERS parameters = {0};
			parameters.DirtyRectsCount = 0;
			parameters.pDirtyRects = NullPtr;
			parameters.pScrollRect = NullPtr;
			parameters.pScrollOffset = NullPtr;

			// The first argument instructs DXGI to block until VSync, putting the application
			// to sleep until the next VSync. This ensures we don't waste any cycles rendering
			// frames that will never be displayed to the screen.
			HRESULT hr = robj->SwapChain->Present1(1, 0, &parameters);

			// Discard the contents of the render target.
			// This is a valid operation only when the existing contents will be entirely
			// overwritten. If dirty or scroll rects are used, this call should be removed.
			robj->Context->DiscardView(robj->RenderTargetView.Get());

			// Discard the contents of the depth stencil.
			robj->Context->DiscardView(robj->DepthStencilView.Get());

			// If the device was removed either by a disconnect or a driver upgrade, we 
			// must recreate all device resources.
			if (hr == DXGI_ERROR_DEVICE_REMOVED)
			{
				handleDeviceLost();
			}
			else
			{
				DX_ThrowIfFailed(hr);
			}

			return Success;
		}

		// 버텍스 버퍼
		vertex_buffer_t createVertexBuffer(uint32 elementCount, const void *vertexBuffer, const vertex_element_t vertexElementFormatID, int usage)
		{
			return GY_INVALID_VERTEX_BUFFER_ID;
		}
		
		vertex_element_t createVertexElementFormat(uint32 elementCount, const int *vertexElementFormats)
		{
			if (vertexElementFormats == NullPtr)
				return GY_INVALID_VERTEX_ELEMENT_ID;

			return GY_INVALID_VERTEX_ELEMENT_ID;
		}

		// 인덱스 버퍼
		index_buffer_t createIndexBuffer(uint32 sizeOfElementType, uint32 elementCount, const void *indexBuffer, int usage)
		{
			return GY_INVALID_INDEX_BUFFER_ID;
		}

		// 텍스쳐 버퍼
		texture_t createTextureFromFile(const char *fileName, uint32 width, uint32 height)
		{
			return GY_INVALID_TEXTURE_ID;
		}

		texture_t createTextureFromMemory(void *rawBuffer, uint32 width, uint height)
		{
			return GY_INVALID_TEXTURE_ID;
		}

		int bindTexture(texture_t tid)
		{
			return Success;
		}

		int destroyTexture(texture_t tid)
		{
			return Success;
		}

		// 셰이더
		shader_t createShaderFromFile(shader_type_e shaderProgramType, const char *fileName)
		{
			return GY_INVALID_SHADER_ID;
		}

		shader_t createShaderFromMemory(shader_type_e shaderProgramType, const char *buffer)
		{
			return GY_INVALID_SHADER_ID;
		}

		// 그리기
		int draw(draw_type_e drawType, uint32 sizeOfVertices, vertex_buffer_t vbid)
		{
			return Success;
		}

		int drawIndexed(draw_type_e drawType, uint32 sizeOfIndexType, vertex_buffer_t vbid, index_buffer_t ibid)
		{
			return Success;
		}
	}
}
#endif