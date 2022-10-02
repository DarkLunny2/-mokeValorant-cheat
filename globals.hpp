#pragma once
#include <Windows.h>

HWND Entryhwnd = NULL;
int processid = 0;

int process_id;
DWORD64 image_base;


/////////////////////////////////added////////////////////////
bool InLobby = false;
std::string string21337;

namespace globals {
    static HANDLE handleOffset;
    static uintptr_t pid;
    static uintptr_t base;
    static UINT_PTR
        localplayercontroller,
        localplayerpawn,
        localdamagehandler,
        localcameramanager;
    static int localteamid;


}
///////////////////////////////here///////////////////////

namespace offsets
{
    DWORD

        Gameinstance = 0x1A0,
        Ulevel = 0x38,
        PersistentLevel = 0x40,
        LocalPlayers = 0x40,
        PlayerController = 0x38,
        PlayerCameraManager = 0x478,
        LocalPlayerArray = 0x40,
        MyHUD = 0x470,
        AcknowledgedPawn = 0x460,
        PlayerState = 0x3f0,
        TeamComponent = 0x628,
        TeamID = 0xf8,
        UniqueID = 0x38,
        FNameID = 0x18,
        ActorArray = 0xA0,
        AActorArray = 0xa0,
        ActorCount = 0xB8,
        RootComponent = 0x230,
        RelativeLocation = 0x164,
        MeshComponent = 0x430,
        DamageHandler = 0x9a8,
        root_position = 0x164,
        bIsDormant = 0x100,
        Health = 0x1b0,
        ComponentToWorld = 0x250,
        component_to_world = 0x250,
        BoneArray = 0x5b0,
        BoneArrayCache = BoneArray + 0x10,
        BoneCount = 0x5b8,



        camera_position = 0x1260,			// world > game_instance > localplayers_array[0] > playercontroller > camera_controller > camera_position
        camera_rotation = 0x126C,			// world > game_instance > localplayers_array[0] > playercontroller > camera_controller > camera_rotation
        camera_fov = 0x1278,				// world > game_instance > localplayers_array[0] > playercontroller > camera_controller > camera_fov

        last_submit_time = 0x378,
        last_render_time = 0x37C,
        Inventory = 0x948,
        CurrentWeapon = 0x238,
        ObjID=0x18,
        playername = 0x8E88240,
        Spiketime = 0x8C7A800,
       
        CameraLocation = 0x1260;
}

namespace Settings
{

    inline bool bMenu = true;
    inline bool fovchanger = false;
    inline float fovchangervalue = 100.0f;

    namespace Visuals
    {
        inline bool bSnaplines = true;
        inline bool bDistance = false;
        inline bool bboxv2 = false;
        inline bool bBox = true;
        inline bool bBoxOutlined = false;
        inline bool bHealth = false;
        inline bool player_skeleton = false;


        inline  int selectedhitbox = 0;
        inline  const char* hitbox[] = { ("Head") ,("Neck"), ("Body") };
        
        float aim_constant_offset = 0.f;
        static int BoxWidth = 1.0f;

        //added by Devil
        bool player_box = false;
        bool player_box_3d = false;
       
        //new added
        inline bool headcircle = false;
        inline  int headboxtype = 0;
        inline  const char* headboxt[] = { ("Circle") ,("Filled Circle"), ("Dot") };

       

        inline bool health = false;
        inline bool player_healthbar = false;

        inline bool aimbot = false;
        inline float fov = 150;
        inline float smoothness = 1.0f;
        inline bool smooth = false;
        inline bool drawfov = false;
        inline bool vischeck = false;
        inline bool filledfov = false;
      



        bool player_view_angle = false;

    }

    namespace aimbot
    {
     

        inline bool aimbot = false;
        inline float fov = 150;
        inline float smoothness = 1.0f;
        inline bool drawfov = false;
        inline bool smooth = false;
        
        inline bool vischeck = false;
        inline bool filledfov = false;
        inline  int selectedhitbox = 0;
        inline  const char* hitbox[] = { ("Head") ,("Neck"), ("Body") };
        inline bool silent = false;
        inline bool rcs = false;
        inline bool rcscrosshair = false;
        inline bool nazicrosshair = false;
        inline bool rapidfire = false;
    }

    namespace visuals
    {
        inline bool skeleton = false;
        inline bool snapline = false;

        inline  int snaplinepos = 2;
        inline  const char* snappos[] = { ("Top") ,("Crosshair"), ("Bottom") };

        inline  int healthtype = 0;
        inline  const char* healtht[] = { ("Text") ,("Bar"), ("Both") };

        inline  int headboxtype = 0;
        inline  const char* headboxt[] = { ("Circle") ,("Filled Circle"), ("Dot") };

        inline bool headcircle = false;
        inline bool distance = false;
        inline bool box = false;
        inline bool box3d = false;
        inline bool d3box = false;
        inline bool weapon = true;
        inline bool glow = false;
        int glowintensity = 100;
        inline bool radar = false;
        inline bool dorment = false;
        inline bool health = false;
        inline bool filledbox = false;
        inline bool rendercount = false;
        inline bool agentesp = false;
        inline bool weaponesp = false;
        inline bool ammoesp = false;
        inline bool spiketimer = false;
        inline bool weaponstate = false;
        inline bool textesp = false;
        inline bool shield = false;
        inline bool jointesp = false;
        inline bool visibleesp = false;
        inline bool nameesp = false;
        inline bool teamswap = false;
        inline bool chams = false;
    }
}


/////////////////////////////////////////////////////////////////////////////

namespace visualsteam
{
    inline bool teambox = false;
    inline bool snaplineteam = false;
    inline  int snaplineposteam = 2;
    inline  const char* snapposteam[] = { ("Top") ,("Crosshair"), ("Bottom") };
    inline bool skeleton = false;
}