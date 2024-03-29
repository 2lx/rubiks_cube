#include "all.h"

#include "playstate.h"
#include "rcubeparams.h"
#include "rcubeobject.h"
#include "shader.h"
#include "shaderprogram.h"
#include <random>
#include <memory>

#include <glm/gtc/matrix_transform.hpp>

PlayState PlayState::m_PlayState;

PlayState::PlayState()
    :
      m_screenWidth( INIT_SCREEN_WIDTH ),
      m_screenHeight( INIT_SCREEN_HEIGHT )
{

}

void PlayState::init()
{
    // Setup shader program
    Shader vertexShader( GL_VERTEX_SHADER );
    vertexShader.loadFromFile( "glsl/shader.v.glsl" );
    vertexShader.compile();

    Shader fragmentShader( GL_FRAGMENT_SHADER );
    fragmentShader.loadFromFile( "glsl/shader.f.glsl" );
    fragmentShader.compile();

    m_shaderPr = std::unique_ptr<ShaderProgram>( new ShaderProgram() );
    m_shaderPr->attachShader( vertexShader );
    m_shaderPr->attachShader( fragmentShader );
    m_shaderPr->linkProgram();

    m_RCube = std::unique_ptr<RCubeObject>( new RCubeObject( m_shaderPr.get() ) );

    glGenBuffers( 1, &m_VBOScreenVertices );
    glBindBuffer( GL_ARRAY_BUFFER, m_VBOScreenVertices );
    glBufferData( GL_ARRAY_BUFFER, sizeof( m_aScreenVertices ), m_aScreenVertices, GL_STATIC_DRAW );

    m_attrScreenVertices = m_shaderPr->addAttribute( "scrVertex" );
    m_UniIsBG = m_shaderPr->addUniform( "isBG" );
    m_UniPlasmaSpeed = m_shaderPr->addUniform( "plasmaSpeed" );

    glEnable( GL_BLEND );
    glEnable( GL_DEPTH_TEST );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    setProjection( m_prType );
}

void PlayState::cleanup()
{

}

void PlayState::setProjection( const PT pt )
{
    const float vMargin = ( 1 + RC::CUBIE_COUNT );
    const float hMargin = vMargin * ( float ) m_screenWidth / ( float ) m_screenHeight;

    m_mProjection = glm::ortho( -hMargin, hMargin, -vMargin, vMargin, 0.0f, 40.0f );
    m_mModel = glm::translate( glm::mat4( 1.0f ), glm::vec3( 0.0f, 0.5f, -20.0f ) );

    if ( pt == PT::Isometric )
        m_mView =
            glm::rotate( glm::mat4( 1.0f ), glm::radians( 35.264f ), glm::vec3( 1, 0, 0 ) ) * // X axis
            glm::rotate( glm::mat4( 1.0f ), glm::radians( 45.0f ), glm::vec3( 0, 1, 0 ) ) * // Y axis
            glm::rotate( glm::mat4( 1.0f ), glm::radians( 0.0f ), glm::vec3( 0, 0, 1 ) );  // Z axis
    else if ( pt == PT::Dimetric )
        m_mView = {
            // cavalier view
            1.0f,     0.0f, 0.0f, 0.0f,
            0.0f,     1.0f, 0.0f, 0.0f,
         0.3345f, -0.3345f, 1.0f, 0.0f,
            0.0f,     0.0f, 0.0f, 1.0f
    };

    m_matrCamera = m_mProjection * m_mModel * m_mView;
}

void PlayState::pause()
{

}

void PlayState::resume()
{

}

void PlayState::handleEvents( GameEngine* game )
{
    static bool lastEvent = false;
    SDL_Event event;

    const Uint32 start = SDL_GetTicks();
    bool allEventsRunOut = false;

    while ( SDL_PollEvent( &event ) && !allEventsRunOut && ( SDL_GetTicks() - start ) < 15 )
    {
        switch( event.type )
        {
        case SDL_QUIT:
            game->quit();
            break;

        case SDL_WINDOWEVENT:
            if ( event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED )
            {
                m_screenWidth = event.window.data1;
                m_screenHeight = event.window.data2;

                glViewport( 0, 0, m_screenWidth, m_screenHeight );
                setProjection( m_prType );
                m_needRedraw = true;
            }
            break;

        case SDL_KEYDOWN:
            lastEvent = true;
            switch( event.key.keysym.sym )
            {
            case SDLK_ESCAPE:
                game->quit();
                break;

            // rotates
            case SDLK_UP:       m_keyQ.keyDown( RC::GK::RotateUp ); break;
            case SDLK_DOWN:     m_keyQ.keyDown( RC::GK::RotateDown ); break;
            case SDLK_LEFT:     m_keyQ.keyDown( RC::GK::RotateLeft ); break;
            case SDLK_RIGHT:    m_keyQ.keyDown( RC::GK::RotateRight ); break;
            case SDLK_PAGEDOWN: m_keyQ.keyDown( RC::GK::RotateACW ); break;
            case SDLK_DELETE:   m_keyQ.keyDown( RC::GK::RotateCW ); break;

            // turns
            case SDLK_e:
                if ( event.key.keysym.mod & KMOD_CTRL )
                    m_keyQ.keyDown( RC::GK::TurnFI );
                else m_keyQ.keyDown( RC::GK::TurnF );
                break;
            case SDLK_q:
                if ( event.key.keysym.mod & KMOD_CTRL )
                    m_keyQ.keyDown( RC::GK::TurnBI );
                else m_keyQ.keyDown( RC::GK::TurnB );
                break;
            case SDLK_w:
                if ( event.key.keysym.mod & KMOD_CTRL )
                    m_keyQ.keyDown( RC::GK::TurnUI );
                else m_keyQ.keyDown( RC::GK::TurnU );
                break;
            case SDLK_s:
                if ( event.key.keysym.mod & KMOD_CTRL )
                    m_keyQ.keyDown( RC::GK::TurnDI );
                else m_keyQ.keyDown( RC::GK::TurnD );
                break;
            case SDLK_a:
                if ( event.key.keysym.mod & KMOD_CTRL )
                    m_keyQ.keyDown( RC::GK::TurnLI );
                else m_keyQ.keyDown( RC::GK::TurnL );
                break;
            case SDLK_d:
                if ( event.key.keysym.mod & KMOD_CTRL )
                    m_keyQ.keyDown( RC::GK::TurnRI );
                else m_keyQ.keyDown( RC::GK::TurnR );
                break;

            // other
            case SDLK_F1:   m_keyQ.keyDown( RC::GK::GameReset );    break;
            case SDLK_F4:   m_keyQ.keyDown( RC::GK::GameMix );      break;
            case SDLK_F5:   m_keyQ.keyDown( RC::GK::GameBG );       break;
            case SDLK_F6:   m_keyQ.keyDown( RC::GK::GameProj );     break;
            case SDLK_F7:   m_keyQ.keyDown( RC::GK::GameColor );    break;
            case SDLK_z:    m_keyQ.keyDown( RC::GK::GameUndo );     break;
            }
            break;
        case SDL_KEYUP:
            lastEvent = true;
            switch( event.key.keysym.sym )
            {
            // rotates
            case SDLK_UP:       m_keyQ.keyUp( RC::GK::RotateUp );   break;
            case SDLK_DOWN:     m_keyQ.keyUp( RC::GK::RotateDown ); break;
            case SDLK_LEFT:     m_keyQ.keyUp( RC::GK::RotateLeft ); break;
            case SDLK_RIGHT:    m_keyQ.keyUp( RC::GK::RotateRight );break;
            case SDLK_PAGEDOWN: m_keyQ.keyUp( RC::GK::RotateACW );  break;
            case SDLK_DELETE:   m_keyQ.keyUp( RC::GK::RotateCW );   break;

            // turns
            case SDLK_e:
                if ( event.key.keysym.mod & KMOD_CTRL )
                    m_keyQ.keyUp( RC::GK::TurnFI );
                else m_keyQ.keyUp( RC::GK::TurnF );
                break;
            case SDLK_q:
                if ( event.key.keysym.mod & KMOD_CTRL )
                    m_keyQ.keyUp( RC::GK::TurnBI );
                else m_keyQ.keyUp( RC::GK::TurnB );
                break;
            case SDLK_w:
                if ( event.key.keysym.mod & KMOD_CTRL )
                    m_keyQ.keyUp( RC::GK::TurnUI );
                else m_keyQ.keyUp( RC::GK::TurnU );
                break;
            case SDLK_s:
                if ( event.key.keysym.mod & KMOD_CTRL )
                    m_keyQ.keyUp( RC::GK::TurnDI );
                else m_keyQ.keyUp( RC::GK::TurnD );
                break;
            case SDLK_a:
                if ( event.key.keysym.mod & KMOD_CTRL )
                    m_keyQ.keyUp( RC::GK::TurnLI );
                else m_keyQ.keyUp( RC::GK::TurnL );
                break;
            case SDLK_d:
                if ( event.key.keysym.mod & KMOD_CTRL )
                    m_keyQ.keyUp( RC::GK::TurnRI );
                else m_keyQ.keyUp( RC::GK::TurnR );
                break;

            // other
            case SDLK_F1:   m_keyQ.keyUp( RC::GK::GameReset ); break;
            case SDLK_F4:   m_keyQ.keyUp( RC::GK::GameMix ); break;
            case SDLK_F5:   m_keyQ.keyUp( RC::GK::GameBG ); break;
            case SDLK_F6:   m_keyQ.keyUp( RC::GK::GameProj ); break;
            case SDLK_F7:   m_keyQ.keyUp( RC::GK::GameColor ); break;
            case SDLK_z:    m_keyQ.keyUp( RC::GK::GameUndo ); break;
            }
            break;
        // mouse events
        case SDL_MOUSEBUTTONDOWN:
            lastEvent = true;
            switch( event.button.button )
            {
            case SDL_BUTTON_LEFT:
                m_pMBegin = getGLPos( event.button.x, event.button.y );

                m_keyQ.keyDown( RC::GK::MouseMove, false );
                break;
            case SDL_BUTTON_RIGHT:
                m_pRBegin = getGLPos( event.button.x, event.button.y );
                m_keyQ.keyDown( RC::GK::MouseRotate, false );
                break;
            }
            break;
        case SDL_MOUSEMOTION:
            lastEvent = true;
            switch( event.button.button )
            {
            case SDL_BUTTON_LEFT:
                if ( glm::length( m_pMBegin ) > 0 )
                    m_pMEnd = getGLPos( event.button.x, event.button.y );
                break;
            case SDL_BUTTON( SDL_BUTTON_RIGHT ): // SDL2 bug
                if ( glm::length( m_pRBegin ) > 0 )
                    m_pREnd = getGLPos( event.button.x, event.button.y );
                break;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            lastEvent = true;
            switch( event.button.button )
            {
            case SDL_BUTTON_LEFT:
                m_keyQ.keyUp( RC::GK::MouseMove );
                m_pMBegin = { 0.0f, 0.0f, 0.0f };
                m_pMEnd = { 0.0f, 0.0f, 0.0f };
                break;
            case SDL_BUTTON_RIGHT:
                m_keyQ.keyUp( RC::GK::MouseRotate );
                m_pRBegin = { 0.0f, 0.0f, 0.0f };
                m_pREnd = { 0.0f, 0.0f, 0.0f };
                break;
            }
            break;
        default:
            if ( lastEvent )
                allEventsRunOut = true;

            break;
        }
    }
}

glm::vec3 PlayState::getGLPos( const int mX, const int mY ) const
{
    GLint viewport[ 4 ];
    glGetIntegerv( GL_VIEWPORT, viewport ); // 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT

    glm::dmat4 modelView = m_mModel * m_mView;

    GLfloat winX, winY, winZ;
    winX = ( float ) mX;
    winY = ( float ) viewport[ 3 ] - ( float ) mY;
    glReadPixels( winX, winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

    GLdouble posX, posY, posZ;
    gluUnProject( winX, winY, winZ,
        glm::value_ptr( modelView ), glm::value_ptr( m_mProjection ), viewport,
        &posX, &posY, &posZ );

    return glm::vec3( posX, posY, posZ );
}

void PlayState::update( GameEngine * game )
{
    // primarily processing mouse events
    if ( m_keyQ.isHold( RC::GK::MouseMove ) )
    {
        // processing cube turns with mouse
        if ( !m_RCube->isTurning() && !m_RCube->isRotating() )
        {
            if ( glm::distance( m_pMBegin, m_pMEnd ) > 0.3 && glm::length( m_pMBegin ) > 0 && glm::length( m_pMEnd ) > 0 )
            {
                RC::TT mt = m_RCube->setTurnByCoords( m_pMBegin, m_pMEnd );
                m_keyQ.prevPush( RC::GKPar::fromTT( mt ) );
                m_keyQ.processKey( RC::GK::MouseMove );

                m_pMBegin = { 0.0f, 0.0f, 0.0f };
                m_pMEnd = { 0.0f, 0.0f, 0.0f };
            }
        }
    }
    else if ( m_keyQ.isHold( RC::GK::MouseRotate ) )
    {
        // processing cube rotates with mouse
        if ( !m_RCube->isRotating() )
        {
            if ( glm::distance( m_pRBegin, m_pREnd ) > 0.3 && glm::length( m_pRBegin ) > 0 && glm::length( m_pREnd ) > 0 )
            {
                RC::RT rt = m_RCube->setRotateByCoords( m_pRBegin, m_pREnd );
                m_keyQ.prevPush( RC::GKPar::fromRT( rt ) );
                m_keyQ.processKey( RC::GK::MouseRotate );

                m_pRBegin = { 0.0f, 0.0f, 0.0f };
                m_pREnd = { 0.0f, 0.0f, 0.0f };
            }
        }
    }
    else if ( m_keyQ.qSize() > 0 )
    {
        const RC::GK gk = m_keyQ.qCurKey();
        const bool enableAll = !m_RCube->isTurning() && !m_RCube->isRotating();

        if ( RC::GKPar::enableWithTurn( gk ) || enableAll )
        {
            m_keyQ.qPopKey();

            switch ( gk )
            {
            // processing cube rotates
            case RC::GK::RotateDown:
            case RC::GK::RotateUp:
            case RC::GK::RotateRight:
            case RC::GK::RotateLeft:
            case RC::GK::RotateACW:
            case RC::GK::RotateCW:
                m_RCube->setRotate( RC::GKPar::toRT( gk ) );
                break;
            // processing cube turns
            case RC::GK::TurnB:
            case RC::GK::TurnBI:
            case RC::GK::TurnD:
            case RC::GK::TurnDI:
            case RC::GK::TurnF:
            case RC::GK::TurnFI:
            case RC::GK::TurnL:
            case RC::GK::TurnLI:
            case RC::GK::TurnR:
            case RC::GK::TurnRI:
            case RC::GK::TurnU:
            case RC::GK::TurnUI:
            case RC::GK::TurnFM:
            case RC::GK::TurnFMI:
            case RC::GK::TurnUM:
            case RC::GK::TurnUMI:
            case RC::GK::TurnRM:
            case RC::GK::TurnRMI:
                m_RCube->setTurn( RC::GKPar::toTT( gk ) );
                break;
                // processing projection setup
            case RC::GK::GameProj:
                m_prType = PT ( ( ( int ) m_prType + 1 ) % ( int ) PT::Count );
                setProjection( m_prType );
                m_needRedraw = true;
                break;
                // processing colors setup
            case RC::GK::GameColor:
                m_RCube->incCurScheme();
                m_needRedraw = true;
                break;
                // other
            case RC::GK::GameReset:
                m_RCube->reset();
                m_needRedraw = true;
                break;
            case RC::GK::GameMix:
            {
                static std::default_random_engine dre(time(0));
                std::uniform_int_distribution<int> distType(0, 10),
                            distRT(0, int( RC::RT::Count ) - 1),
                            distTT(0, int( RC::TT::Count ) - 1);

                for ( int i = 0; i < 60; i++ )
                {
                    if ( distType(dre) > 6 )
                    {
                        const RC::RT rRand = RC::RT( distRT(dre) );
                        m_keyQ.qPushKey( RC::GKPar::fromRT( rRand ) );
                    }
                    else
                    {
                        const RC::TT tRand = RC::TT( distTT(dre) );
                        m_keyQ.qPushKey( RC::GKPar::fromTT( tRand ) );
                    }
                }

                break;
            }
            case RC::GK::GameUndo:
            {
                const RC::GK pgk = m_keyQ.prevPop();
                const RC::GK gk = RC::GKPar::prevGK( pgk );
                if ( gk == RC::GK::None )
                    break;

                std::cout << RC::GKPar::str( pgk ) << " -> " << RC::GKPar::str( gk ) << std::endl;

                if ( RC::GKPar::toRT( gk ) != RC::RT::None )
                    m_RCube->setRotate( RC::GKPar::toRT( gk ) );
                else if ( RC::GKPar::toTT( gk ) != RC::TT::None )
                    m_RCube->setTurn( RC::GKPar::toTT( gk ) );

                break;
            }
            case RC::GK::GameBG:
                m_trBG = !m_trBG;
                m_needRedraw = true;
                break;
            default:
                break;
            }
        }
    }

    m_RCube->update();
}

void PlayState::drawBackground()
{
    glUniform1f( m_UniIsBG, 1.0 );

    glEnableVertexAttribArray( m_attrScreenVertices );
    glBindBuffer( GL_ARRAY_BUFFER, m_VBOScreenVertices );
    glVertexAttribPointer( m_attrScreenVertices, 2, GL_FLOAT, GL_FALSE, 0, 0 );

    glDrawArrays( GL_QUADS, 0, 4 );

    glDisableVertexAttribArray( m_attrScreenVertices );

    glUniform1f( m_UniIsBG, 0.0 );
}

void PlayState::draw( GameEngine * game )
{
    static int drCount = 0;

    if ( m_needRedraw || m_RCube->isRotating() || m_RCube->isTurning() || m_trBG )
    {
        Uint32 start = SDL_GetTicks();
        drCount++;

        if ( m_trBG )
            glUniform1f( m_UniPlasmaSpeed, float( drCount ) );
        else
        {
            glClearColor( 47.0 / 255.0, 47.0 / 255.0, 47.0 / 255.0, 0.0f );
            glUniform1f( m_UniPlasmaSpeed, 0.0 );
        }

        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        glUseProgram( m_shaderPr->id() );

        m_RCube->rotateObject();

        drawBackground();
        m_RCube->drawObject( m_matrCamera );

        if ( SDL_GetTicks() - start < SCREEN_TICK_PER_FRAME )
            SDL_Delay( SCREEN_TICK_PER_FRAME - ( SDL_GetTicks() - start ) );

        if ( drCount > 1 ) m_needRedraw = false;
#ifdef MY_DEBUG
//      if ( drCount % 5 == 0 )
//          std::cout << "DrawCount: " << drCount << std::endl;
#endif // MY_DEBUG
    }
}

