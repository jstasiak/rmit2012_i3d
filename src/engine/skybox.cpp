#include "precompile.h"

#include "skybox.h"

#include "gameobject/camera.h"
#include "texture.h"

Skybox::Skybox(std::string fileName)
	: texture(new Texture(fileName))
{
}

void Skybox::drawForCamera(std::shared_ptr< Camera > camera) {
	this->texture->activate();

	auto forward = camera->getForward();

	// Store the current matrix
    glPushMatrix();

    // Reset and transform the matrix.
    glLoadIdentity();
    gluLookAt(
        0, 0, 0,
        forward.x, forward.y, forward.z,
        0, 1, 0);

    // Enable/Disable features
    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);

	float dx = 1.0f / 4.0f;
	float dy = 1.0f / 3.0f;

	float basex = 0.0f;
	float basey = 0.0f;

	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glScalef(1.0f, -1.0f, 1.0f);
	

    // Just in case we set all vertices to white.
    glColor4f(1,1,1,1);


    // Render the front quad
    glBegin(GL_QUADS);
		basex = 1 * dx;
		basey = 1 * dy;
        glTexCoord2f(basex + 0, basey + 0); glVertex3f(  0.5f, -0.5f, -0.5f );
        glTexCoord2f(basex + dx, basey + 0); glVertex3f( -0.5f, -0.5f, -0.5f );
        glTexCoord2f(basex + dx, basey + dy); glVertex3f( -0.5f,  0.5f, -0.5f );
        glTexCoord2f(basex + 0, basey + dy); glVertex3f(  0.5f,  0.5f, -0.5f );

    // Render the left quad
		basex = 0 * dx;
		basey = 1 * dy;
        glTexCoord2f(basex + 0, basey + 0); glVertex3f(  0.5f, -0.5f,  0.5f );
        glTexCoord2f(basex + dx, basey + 0); glVertex3f(  0.5f, -0.5f, -0.5f );
        glTexCoord2f(basex + dx, basey + dy); glVertex3f(  0.5f,  0.5f, -0.5f );
        glTexCoord2f(basex + 0, basey + dy); glVertex3f(  0.5f,  0.5f,  0.5f );


    // Render the back quad
		basex = 3 * dx;
		basey = 1 * dy;
        glTexCoord2f(basex + 0, basey + 0); glVertex3f( -0.5f, -0.5f,  0.5f );
        glTexCoord2f(basex + dx, basey + 0); glVertex3f(  0.5f, -0.5f,  0.5f );
        glTexCoord2f(basex + dx, basey + dy); glVertex3f(  0.5f,  0.5f,  0.5f );
        glTexCoord2f(basex + 0, basey + dy); glVertex3f( -0.5f,  0.5f,  0.5f );


    // Render the right quad
		basex = 2 * dx;
		basey = 1 * dy;
        glTexCoord2f(basex + 0, basey + 0); glVertex3f( -0.5f, -0.5f, -0.5f );
        glTexCoord2f(basex + dx, basey + 0); glVertex3f( -0.5f, -0.5f,  0.5f );
        glTexCoord2f(basex + dx, basey + dy); glVertex3f( -0.5f,  0.5f,  0.5f );
        glTexCoord2f(basex + 0, basey + dy); glVertex3f( -0.5f,  0.5f, -0.5f );


    // Render the top quad
		basex = 1 * dx;
		basey = 2 * dy;
        glTexCoord2f(basex + 0, basey + dy); glVertex3f( -0.5f,  0.5f, -0.5f );
        glTexCoord2f(basex + 0, basey + 0); glVertex3f( -0.5f,  0.5f,  0.5f );
        glTexCoord2f(basex + dx, basey + 0); glVertex3f(  0.5f,  0.5f,  0.5f );
        glTexCoord2f(basex + dx, basey + dy); glVertex3f(  0.5f,  0.5f, -0.5f );

    // Render the bottom quad
		basex = 1 * dx;
		basey = 0 * dy;
        glTexCoord2f(basex + 0, basey + 0); glVertex3f( -0.5f, -0.5f, -0.5f );
        glTexCoord2f(basex + 0, basey + dy); glVertex3f( -0.5f, -0.5f,  0.5f );
        glTexCoord2f(basex + dx, basey + dy); glVertex3f(  0.5f, -0.5f,  0.5f );
        glTexCoord2f(basex + dx, basey + 0); glVertex3f(  0.5f, -0.5f, -0.5f );
    glEnd();

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

    // Restore enable bits and matrix
    glPopAttrib();
    glPopMatrix();

	this->texture->deactivate();
}