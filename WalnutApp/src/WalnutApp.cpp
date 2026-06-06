#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"

class ExampleLayer : public Walnut::Layer
{
public:
	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");
		if (ImGui::Button("Render Image"))
		{
			Render();
		}
		ImGui::End();

		ImGui::Begin("ViewPort");

		mViewportW = ImGui::GetContentRegionAvail().x;
		mViewportH = ImGui::GetContentRegionAvail().y;

		if (mImage)
			ImGui::Image(mImage->GetDescriptorSet(), { (float)mImage->GetWidth(), (float)mImage->GetHeight() });

		ImGui::End();
	}

	void Render()
	{
		//If there is no image or a size change
		if (!mImage || mViewportW != mImage->GetWidth() || mViewportH != mImage->GetHeight())
		{
			//Remake the image
			mImage = std::make_shared<Walnut::Image>(mViewportW, mViewportH, Walnut::ImageFormat::RGBA);
			//Allocate enough memory for mImageData
			mImageData = std::make_unique<uint32_t[]>(mViewportW * mViewportH);
		}

		for (uint32_t i = 0; i < mViewportW * mViewportH; i++)
		{
			mImageData[i] = 0xffff00ff;
		}

		//.get just passes the memory from the pointer to the function
		mImage->SetData(mImageData.get());
	}

private:

	//Using shared here because Image is needed alive for many GPU resources
	std::shared_ptr<Walnut::Image> mImage;
	
	//Stores pixel data. Using unique_ptr because it only owns one piece of data
	std::unique_ptr<uint32_t[]> mImageData = nullptr;
	
	uint32_t mViewportW = 0;
	uint32_t mViewportH = 0;

};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Walnut Example";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}