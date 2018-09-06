#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QMainWindow>
#include <QSystemTrayIcon>

namespace Ui{
class MusicPlayer;
}

class MusicPlayer : public QMainWindow
{
public:
    MusicPlayer();
};

#endif // MUSICPLAYER_H
