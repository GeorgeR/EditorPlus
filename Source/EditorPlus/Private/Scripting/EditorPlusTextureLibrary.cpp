#include "Scripting/EditorPlusTextureLibrary.h"

#include "Engine/Texture.h"
#include "Engine/Texture2D.h"

FLinearColor UEditorPlusTextureLibrary::GetAverageColor(UTexture2D* Texture)
{
	const auto CompressionSettings = Texture->CompressionSettings;
	const auto MipGenSettings = Texture->MipGenSettings;
	const auto SRGB = Texture->SRGB;

	Texture->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
	Texture->MipGenSettings = TextureMipGenSettings::TMGS_FromTextureGroup;
	Texture->SRGB = false;
	Texture->UpdateResource();

	const auto* ImageData = StaticCast<const FColor*>(Texture->PlatformData->Mips.Last().BulkData.LockReadOnly());

	auto Average = ImageData[0];
	for(auto X = 0; X < Texture->GetSizeX(); X++)
	{
		for(auto Y = 0; Y < Texture->GetSizeY(); Y++)
		{
			const auto Pixel = ImageData[Y * Texture->GetSizeX() + X];

			auto Red = Pixel.R;
			auto Green = Pixel.G;
			auto Blue = Pixel.B;
			auto Alpha = Pixel.A;

			Red = (Average.R + Red) / 2;
			Green = (Average.G + Green) / 2;
			Blue = (Average.B + Blue) / 2;
			Alpha = (Average.A + Alpha) / 2;
			
			Average = FColor(Red, Green, Blue, Alpha);
		}
	}

	Texture->PlatformData->Mips.Last().BulkData.Unlock();

	Texture->CompressionSettings = CompressionSettings;
	Texture->MipGenSettings = MipGenSettings;
	Texture->SRGB = SRGB;
	Texture->UpdateResource();

	return Average.ReinterpretAsLinear();
}
