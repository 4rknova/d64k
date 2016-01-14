#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <GL/gl.h>

#define STRINGIFY(A) #A

char *vsh =
#include "demo.vsh"

char *fsh=
#include "demo.fsh"

PIXELFORMATDESCRIPTOR pfd={0,1,PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 0, 0, 0, 0, 0, 0, 0};
DEVMODE dmScreenSettings={ 0,0,0,sizeof(DEVMODE),0,DM_PELSWIDTH|DM_PELSHEIGHT, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1024,768,0,0,0,0,0,0,0,0,0,0};

#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER   0x8B31
typedef char GLchar;

typedef void   (APIENTRY * PFNGLCOMPILESHADERPROC) (GLuint shader);
typedef void   (APIENTRY * PFNGLATTACHSHADERPROC ) (GLuint program, GLuint shader);
typedef GLuint (APIENTRY * PFNGLCREATEPROGRAMPROC) (void);
typedef GLuint (APIENTRY * PFNGLCREATESHADERPROC ) (GLenum type);
typedef void   (APIENTRY * PFNGLSHADERSOURCEPROC ) (GLuint shader, GLsizei count, GLchar *const *string, const GLint *length);
typedef void   (APIENTRY * PFNGLLINKPROGRAMPROC  ) (GLuint program);
typedef void   (APIENTRY * PFNGLUSEPROGRAMPROC   ) (GLuint program);
#define glCompileShader ((PFNGLCOMPILESHADERPROC) wglGetProcAddress("glCompileShader"))
#define glAttachShader  ((PFNGLATTACHSHADERPROC ) wglGetProcAddress("glAttachShader"))
#define glCreateProgram ((PFNGLCREATEPROGRAMPROC) wglGetProcAddress("glCreateProgram"))
#define glCreateShader  ((PFNGLCREATESHADERPROC ) wglGetProcAddress("glCreateShader"))
#define glShaderSource  ((PFNGLSHADERSOURCEPROC ) wglGetProcAddress("glShaderSource"))
#define glLinkProgram   ((PFNGLLINKPROGRAMPROC  ) wglGetProcAddress("glLinkProgram"))
#define glUseProgram    ((PFNGLUSEPROGRAMPROC   ) wglGetProcAddress("glUseProgram"))

void main(int argc, char **argv)
{
    ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN);
    HDC hDC = GetDC(CreateWindow(L"edit",0,WS_POPUP | WS_VISIBLE | WS_MAXIMIZE, 0, 0, 0, 0, 0, 0, 0, 0));
    SetPixelFormat(hDC, ChoosePixelFormat(hDC, &pfd) , &pfd);
    wglMakeCurrent(hDC, wglCreateContext(hDC));
    ShowCursor(0);
    GLuint p = glCreateProgram(),
           s = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(s, 1, &vsh, 0);
    glCompileShader(s);
    glAttachShader(p,s);
    s = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(s, 1, &fsh, 0);
    glCompileShader(s);
    glAttachShader(p,s);
    glLinkProgram(p);
    glUseProgram(p);
    while (1) {
        int t=GetTickCount();
        glRecti(t,t,-t,-t);
        SwapBuffers(hDC);
        if (GetAsyncKeyState(VK_ESCAPE)) ExitProcess(0);
    }
}