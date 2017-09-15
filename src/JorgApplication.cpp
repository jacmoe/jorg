#include <Corrade/Containers/ArrayView.h>
#include <Corrade/PluginManager/Manager.h>
#include <Magnum/Buffer.h>
#include <Magnum/DefaultFramebuffer.h>
#include <Magnum/Mesh.h>
#include <Magnum/Texture.h>
#include <Magnum/TextureFormat.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Trade/AbstractImporter.h>
#include <Magnum/Trade/ImageData.h>

#include "TexturedTriangleShader.h"
#include "configure.h"

using namespace Magnum;

class JorgApplication: public Platform::Application {
    public:
        explicit JorgApplication(const Arguments& arguments);

    private:
        void drawEvent() override;

        Buffer _buffer;
        Mesh _mesh;
        TexturedTriangleShader _shader;
        Texture2D _texture;
};

JorgApplication::JorgApplication(const Arguments& arguments): Platform::Application{arguments, Configuration{}.setTitle("JORG - Jacmoes Overengineered Roleplaying Game")} {
    using namespace Math::Literals;

    struct TriangleVertex {
        Vector2 position;
        Vector2 textureCoordinates;
    };
    static const TriangleVertex data[]{
        {{-0.5f, -0.5f}, {0.0f, 0.0f}}, /* Left vertex position and texture coordinate */
        {{ 0.5f, -0.5f}, {1.0f, 0.0f}}, /* Right vertex position and texture coordinate */
        {{ 0.0f,  0.5f}, {0.5f, 1.0f}}  /* Top vertex position and texture coordinate */
    };

    _buffer.setData(data, BufferUsage::StaticDraw);
    _mesh.setPrimitive(MeshPrimitive::Triangles)
            .setCount(3)
            .addVertexBuffer(_buffer, 0,
                             TexturedTriangleShader::Position{},
                             TexturedTriangleShader::TextureCoordinates{});

    /* Load TGA importer plugin */
    PluginManager::Manager<Trade::AbstractImporter> manager{MAGNUM_PLUGINS_IMPORTER_DIR};
    std::unique_ptr<Trade::AbstractImporter> importer = manager.loadAndInstantiate("TgaImporter");
    if(!importer) std::exit(1);

    /* Load the texture */
    const Utility::Resource rs{"textured-triangle-data"};
    if(!importer->openData(rs.getRaw("../assets/textures/stone.tga")))
        std::exit(2);

    /* Set texture data and parameters */
    std::optional<Trade::ImageData2D> image = importer->image2D(0);
    CORRADE_INTERNAL_ASSERT(image);
    _texture.setWrapping(Sampler::Wrapping::ClampToEdge)
            .setMagnificationFilter(Sampler::Filter::Linear)
            .setMinificationFilter(Sampler::Filter::Linear)
            .setStorage(1, TextureFormat::RGB8, image->size())
            .setSubImage(0, {}, *image);
}

void JorgApplication::drawEvent() {
    defaultFramebuffer.clear(FramebufferClear::Color);
    _shader.setColor({1.0f, 0.7f, 0.7f})
            .setTexture(_texture);
    _mesh.draw(_shader);
    swapBuffers();
}

MAGNUM_APPLICATION_MAIN(JorgApplication)
