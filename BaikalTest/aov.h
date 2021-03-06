/**********************************************************************
 Copyright (c) 2016 Advanced Micro Devices, Inc. All rights reserved.
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ********************************************************************/
#pragma once

#include "basic.h"

class AovTest : public BasicTest
{
public:
    void SaveAOV(Baikal::Output* aov, std::string const& file_name) const
    {
        std::string path = m_generate ? m_reference_path : m_output_path;
        path.append(file_name);
        
        OIIO_NAMESPACE_USING;
        using namespace RadeonRays;
        
        auto width = aov->width();
        auto height = aov->height();
        std::vector<float3> data(width * height);
        std::vector<float3> data1(width * height);
        aov->GetData(&data[0]);
        
        for (auto y = 0u; y < height; ++y)
        for (auto x = 0u; x < width; ++x)
        {
            
            float3 val = data[(height - 1 - y) * width + x];
            val *= (1.f / val.w);
            data1[y * width + x].x = val.x;
            data1[y * width + x].y = val.y;
            data1[y * width + x].z = val.z;
        }
        
        ImageOutput* out = ImageOutput::create(path);
        
        if (!out)
        {
            throw std::runtime_error("Can't create image file on disk");
        }
        
        ImageSpec spec(width, height, 3, TypeDesc::FLOAT);
        
        out->open(path, spec);
        out->write_image(TypeDesc::FLOAT, &data1[0], sizeof(float3));
        out->close();
        
        delete out;
    }
};

TEST_F(AovTest, Aov_WorldPosition)
{
    auto output_ws = m_factory->CreateOutput(
        m_output->width(), m_output->height()
    );
    
    m_renderer->SetOutput(Baikal::Renderer::OutputType::kWorldPosition,
                         output_ws.get());
    
    ClearOutput();
    ASSERT_NO_THROW(m_controller->CompileScene(m_scene));
        
    auto& scene = m_controller->GetCachedScene(m_scene);
        
    for (auto i = 0u; i < kNumIterations; ++i)
    {
        ASSERT_NO_THROW(m_renderer->Render(scene));
    }
        
    std::ostringstream oss;
    oss << test_name() << ".png";
    SaveAOV(output_ws.get(), oss.str());
    ASSERT_TRUE(CompareToReference(oss.str()));
}

TEST_F(AovTest, Aov_WorldNormal)
{
    auto output_ws = m_factory->CreateOutput(
         m_output->width(), m_output->height()
    );
    
    m_renderer->SetOutput(Baikal::Renderer::OutputType::kWorldGeometricNormal,
                          output_ws.get());
    
    ClearOutput();
    ASSERT_NO_THROW(m_controller->CompileScene(m_scene));
    
    auto& scene = m_controller->GetCachedScene(m_scene);
    
    for (auto i = 0u; i < kNumIterations; ++i)
    {
        ASSERT_NO_THROW(m_renderer->Render(scene));
    }
    
    std::ostringstream oss;
    oss << test_name() << ".png";
    SaveAOV(output_ws.get(), oss.str());
    ASSERT_TRUE(CompareToReference(oss.str()));
}

TEST_F(AovTest, Aov_ShadingNormal)
{
    auto output_ws = m_factory->CreateOutput(
         m_output->width(), m_output->height()
    );
    
    m_renderer->SetOutput(Baikal::Renderer::OutputType::kWorldShadingNormal,
                          output_ws.get());
    
    ClearOutput();
    ASSERT_NO_THROW(m_controller->CompileScene(m_scene));

    auto& scene = m_controller->GetCachedScene(m_scene);
    
    for (auto i = 0u; i < kNumIterations; ++i)
    {
        ASSERT_NO_THROW(m_renderer->Render(scene));
    }
    
    std::ostringstream oss;
    oss << test_name() << ".png";
    SaveAOV(output_ws.get(), oss.str());
    ASSERT_TRUE(CompareToReference(oss.str()));
}

TEST_F(AovTest, Aov_Tangent)
{
    auto output_ws = m_factory->CreateOutput(
         m_output->width(), m_output->height()
    );
    
    m_renderer->SetOutput(Baikal::Renderer::OutputType::kWorldTangent,
                          output_ws.get());
    
    ClearOutput();
    ASSERT_NO_THROW(m_controller->CompileScene(m_scene));
    
    auto& scene = m_controller->GetCachedScene(m_scene);
    
    for (auto i = 0u; i < kNumIterations; ++i)
    {
        ASSERT_NO_THROW(m_renderer->Render(scene));
    }
    
    std::ostringstream oss;
    oss << test_name() << ".png";
    SaveAOV(output_ws.get(), oss.str());
    ASSERT_TRUE(CompareToReference(oss.str()));
}

TEST_F(AovTest, Aov_Bitangent)
{
    auto output_ws = m_factory->CreateOutput(
         m_output->width(), m_output->height()
    );
    
    m_renderer->SetOutput(Baikal::Renderer::OutputType::kWorldBitangent,
                          output_ws.get());
    
    ClearOutput();
    ASSERT_NO_THROW(m_controller->CompileScene(m_scene));
    
    auto& scene = m_controller->GetCachedScene(m_scene);
    
    for (auto i = 0u; i < kNumIterations; ++i)
    {
        ASSERT_NO_THROW(m_renderer->Render(scene));
    }
    
    std::ostringstream oss;
    oss << test_name() << ".png";
    SaveAOV(output_ws.get(), oss.str());
    ASSERT_TRUE(CompareToReference(oss.str()));
}

TEST_F(AovTest, Aov_Albedo)
{
    auto output_ws = m_factory->CreateOutput(
        m_output->width(), m_output->height()
    );
    
    m_renderer->SetOutput(Baikal::Renderer::OutputType::kAlbedo,
                          output_ws.get());
    
    ClearOutput();
    ASSERT_NO_THROW(m_controller->CompileScene(m_scene));
    
    auto& scene = m_controller->GetCachedScene(m_scene);
    
    for (auto i = 0u; i < kNumIterations; ++i)
    {
        ASSERT_NO_THROW(m_renderer->Render(scene));
    }
    
    std::ostringstream oss;
    oss << test_name() << ".png";
    SaveAOV(output_ws.get(), oss.str());
    ASSERT_TRUE(CompareToReference(oss.str()));
}

TEST_F(AovTest, Aov_Uv)
{
    auto output_ws = m_factory->CreateOutput(
      m_output->width(), m_output->height()
    );

    m_renderer->SetOutput(Baikal::Renderer::OutputType::kUv,
                          output_ws.get());

    ClearOutput();
    ASSERT_NO_THROW(m_controller->CompileScene(m_scene));
    
    auto& scene = m_controller->GetCachedScene(m_scene);
    
    for (auto i = 0u; i < kNumIterations; ++i)
    {
        ASSERT_NO_THROW(m_renderer->Render(scene));
    }
    
    std::ostringstream oss;
    oss << test_name() << ".png";
    SaveAOV(output_ws.get(), oss.str());
    ASSERT_TRUE(CompareToReference(oss.str()));
}

TEST_F(AovTest, Aov_Visibility)
{
    auto output_ws = m_factory->CreateOutput(
        m_output->width(), m_output->height()
    );

    m_renderer->SetOutput(Baikal::Renderer::OutputType::kVisibility,
        output_ws.get());

    ClearOutput();
    ASSERT_NO_THROW(m_controller->CompileScene(m_scene));

    auto& scene = m_controller->GetCachedScene(m_scene);

    for (auto i = 0u; i < kNumIterations; ++i)
    {
        ASSERT_NO_THROW(m_renderer->Render(scene));
    }

    std::ostringstream oss;
    oss << test_name() << ".png";
    SaveAOV(output_ws.get(), oss.str());
    ASSERT_TRUE(CompareToReference(oss.str()));
}