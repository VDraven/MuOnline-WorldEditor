#pragma once

#define Vector(a,b,c,d) {(d)[0]=a;(d)[1]=b;(d)[2]=c;}
typedef float vec_t;

typedef vec_t vec2_t[2];
typedef vec_t vec3_t[3];
typedef vec_t vec4_t[4];
typedef vec_t vec34_t[3][4];

struct Matrix4 { float _M[4][4]; };

enum ATT_TYPE
{
	ATT_64K,
	ATT_128K,
	ATT_SERVER_64K,
};

enum OZB_TYPE
{
	OZB_64K,
	OZB_192K
};

enum OBJ_TYPE
{
	OBJ_V0,
	OBJ_V1,
	OBJ_V2,
	OBJ_V3
};

struct OBB_t
{
	vec3_t StartPos;
	vec3_t XAxis;
	vec3_t YAxis;
	vec3_t ZAxis;
};

struct __declspec(align(4)) Data_NpcName
{
	int Type;
	VS2008::string Name;
	DWORD Level;
	DWORD Kind;
	BYTE Elemental;
	BYTE Reserved[3];
};

struct Vertex_t
{
	short  Node;
	vec3_t Position;
};

struct __declspec(align(4)) Normal_t
{
	short	Node;
	vec3_t	Normal;
	short	BindVertex;
};

struct TexCoord_t
{
	float TexCoordU;
	float TexCoordV;
};

struct __declspec(align(4)) Triangle_t
{
	char       Polygon;
	short      VertexIndex[4];
	short      NormalIndex[4];
	short      TexCoordIndex[4];
	//EdgeTriangleIndex, Front, LightMapCoord, LightMapIndexes are unsed
	short      EdgeTriangleIndex[4];
	bool       Front;
	DWORD	Nothing[7];
};

struct TextureScript
{
	bool m_bBright;
	bool m_bHiddenMesh;
	bool m_bStreamMesh;
	bool m_bNoneBlendMesh;
	BYTE m_byShadowMesh;
};

struct __declspec(align(4)) Mesh_t
{
	bool			NoneBlendMesh;
	short			NumVertices;
	short			NumNormals;
	short			NumTexCoords;
	short			NumTriangles;
	short			Texture;
	Vertex_t*		Vertices;
	Normal_t*		Normals;
	TexCoord_t*		TexCoords;
	Triangle_t*		Triangles;
	TextureScript*	m_csTScript;
};

struct __declspec(align(4)) Action_t
{
	bool Loop;
	float PlaySpeed;
	__int16 NumAnimationKeys;
	bool LockPositions;
	//BYTE gap;
	vec3_t* Positions;
};

struct __declspec(align(4)) BoneMatrix_t
{
	vec3_t* Position;
	vec3_t* Rotation;
	vec4_t* Quaternion;
};

struct __declspec(align(4)) Bone_t
{
	char Name[32];
	short Parent;
	char Dummy;
	//BYTE gap;
	BoneMatrix_t* BoneMatrixes;
	char BoundingBox;
	vec3_t BoundingVertices[8];
};

struct Texture_t
{
	VS2008::string FileName;
};

struct __declspec(align(4)) BMD
{
	//char Name[32];	//HookFixBmdName.  change to std::string (size = 24 (VS2019))
	char OldName[8];		//8
	std::string NewName;	//24

	BYTE Version;
	BYTE FlagName;
	short NumBones;
	short NumMeshs;
	short NumActions;
	Mesh_t *Meshs;
	Bone_t *Bones;
	Action_t *Actions;
	Texture_t* Textures;
	GLuint* IndexTexture;
	int* pMem3;
	int flag;
	vec3_t Unk_vec3_68;
	BYTE LightEnable;
	BYTE ContrastEnable;
	//WORD gap_82;
	vec3_t BodyLight;
	DWORD BoneHead;
	int BoneFoot[4];
	float BodyScale;
	vec3_t BodyOrigin;
	vec3_t BodyAngle;
	float BodyHeight;
	char StreamMesh;
	//BYTE Gap_149[3];
	vec3_t ShadowAngle;
	char Skin;
	BYTE HideSkin;
	BYTE Unk_166;
	BYTE Unk_167;
	WORD CurrentAction;
	WORD PriorAction;
	float RenderAnimTimer;
	float CurrentAnimation;
	short CurrentAnimationFrame;
	short Sounds[10];
};


struct __declspec(align(4)) OBJECT
{
	bool Live;
	bool bBillBoard;
	bool m_bCollisionCheck;
	bool m_bRenderShadow;
	bool EnableShadow;
	bool LightEnable1;
	bool LightEnable2;
	bool LightEnable3;
	bool m_bActionStart;
	bool Unk_9;
	bool m_bRenderAfterCharacter;
	bool Visible;
	bool AlphaEnable;
	bool EnableBoneMatrix;
	bool ContrastEnable;
	bool ChromeEnable;
	bool Unk_16;
	bool Unk_17;
	bool Unk_18;
	bool Unk_19;
	DWORD PentagramMainAttribute;
	DWORD Unk_24;
	DWORD Unk_28;
	BYTE Unk_32;
	BYTE Unk_33;
	BYTE Unk_34;
	BYTE Unk_35;
	BYTE Unk_36;
	BYTE Unk_37;
	BYTE Unk_38;
	BYTE Unk_39;
	BYTE Unk_40;
	BYTE AI;
	WORD CurrentAction;
	WORD PriorAction;
	BYTE ExtState;
	BYTE Teleport;
	BYTE Kind;
	BYTE Gap_49;
	WORD Skill;
	BYTE m_byNumCloth;
	BYTE m_byHurtByOneToOne;
	BYTE WeaponLevel;
	BYTE DamageTime;
	BYTE m_byBuildTime;
	BYTE m_bySkillCount;
	BYTE m_bySkillSerialNum;
	BYTE Block;
	void *m_pCloth;
	short ScreenX;
	short ScreenY;
	short PKKey;
	short Weapon;
	int Type;
	int SubType;
	int m_iAnimation;
	int HiddenMesh;
	int LifeTime;
	int Unk_92;
	int BlendMesh;
	int AttackPoint[2];
	int RenderType;
	int InitialSceneFrame;
	int LinkBone;
	DWORD m_dwTime;
	float Scale;
	float Unk_128;
	float BlendMeshLight;
	float BlendMeshTexCoordU;
	float BlendMeshTexCoordV;
	float Timer;
	float m_fEdgeScale;
	float Velocity;
	float CollisionRange;
	float ShadowScale;
	float Gravity;
	float Distance;
	float AnimationFrame;
	float PriorAnimationFrame;
	float AlphaTarget;
	float Alpha;
	vec3_t Light;
	vec3_t Direction;
	vec3_t m_vPosSword;
	vec3_t StartPosition;
	vec3_t BoundingBoxMin;
	vec3_t BoundingBoxMax;
	vec3_t m_vDownAngle;
	vec3_t m_vDeadPosition;
	vec3_t Position;
	vec3_t Angle;
	vec3_t HeadAngle;
	vec3_t HeadTargetAngle;
	vec3_t EyeLeft;
	vec3_t EyeRight;
	vec3_t EyeLeft2;
	vec3_t EyeRight2;
	vec3_t EyeLeft3;
	vec3_t EyeRight3;
	vec3_t v3f_404;
	DWORD Unk_Struct_416[7];
	DWORD Unk_444;
	vec34_t Matrix;
	vec34_t *BoneTransform;
	OBB_t OBB;
	OBJECT *Owner;
	OBJECT *Prior;
	OBJECT *Next;
	void *m_BuffMap;
	DWORD Unk_564;
	DWORD Unk_568;
	DWORD Unk_572;
	DWORD Unk_576;
	DWORD Unk_580;
	DWORD Unk_584;
	DWORD Unk_588;
	DWORD Unk_592;
	short m_sTargetIndex;
	WORD Gap_598;
	DWORD m_iID;
	vec3_t m_v3PrePos1;
	vec3_t m_v3PrePos2;
	DWORD Unk_628[4][6];
	DWORD Unk_724;
	BYTE Unk_728;
	BYTE Gap_729[3];
	BYTE vector_732[24];
	DWORD Unk_756[7];
};

struct __declspec(align(4)) OBJECT_BLOCK
{
	BYTE Index;
	//BYTE gap[3];
	OBJECT *Head;
	OBJECT *Tail;
	bool Visible;
};


struct ObjCreatePrepare
{
	WORD ModelID;
	//WORD GAP;
	vec3_t Position;
	vec3_t Angle;
	vec3_t LightColor;
	float Scale;
	BYTE LightEnable1;
	BYTE LightEnable2;
	BYTE LightEnable3;
	//BYTE GAP;
};

struct __declspec(align(4)) _PART_t
{
	WORD Type;
	BYTE Level;
	BYTE Option1;
	BYTE ExtOption;
	BYTE LinkBone;
	BYTE CurrentAction;
	//BYTE Gap_7;
	WORD PriorAction;
	//WORD Gap_10;
	float AnimationFrame;
	float PriorAnimationFrame;
	float PlaySpeed;
	BYTE m_byNumCloth;
	//BYTE Gap_25[3];
	void* m_pCloth[2];
};

struct PET_INFO
{
	DWORD PetType;
	DWORD Exp;
	DWORD Unk_8;
	WORD Level;
	WORD Dur;
	DWORD Unk_16;
	DWORD Unk_20;
};

struct __declspec(align(4)) _PATH_t
{
	BYTE CurrentPath;
	BYTE CurrentPathFloat;
	BYTE PathNum;
	BYTE PathX[15];
	BYTE PathY[15];
	bool Success;
	bool Error;
	BYTE x;
	BYTE y;
	BYTE Direction;
	BYTE Run;
	BYTE Gap_39;
	DWORD Unk_40;
	DWORD Unk_44;
	WORD X;
	WORD Y;
	DWORD Unk_52;
	WORD Unk_56;
	WORD Unk_58;
	DWORD Unk_60;
	DWORD Unk_64;
	DWORD Unk_68;
	DWORD Unk_72;
	DWORD Unk_76;
	DWORD Unk_80;
	DWORD Unk_84;
};

struct CHARACTER
{
	DWORD pData;
	BYTE SafeZone;
	BYTE Blood;
	BYTE Unk_6;
	BYTE Unk_7;
	DWORD m_bFixForm;
	BYTE Unk_12;
	BYTE Unk_13;
	BYTE Change;
	BYTE Unk_15;
	BYTE m_bIsSelected;
	BYTE Unk_17;
	BYTE Class;
	BYTE Unk_19;
	BYTE CtrlCode;
	BYTE ExtendState;
	BYTE Effect;
	BYTE GuildStatus;
	BYTE GuildType;
	BYTE GuildRelationShip;
	BYTE GuildSkill;
	BYTE GuildMasterKillCount;
	BYTE BackupCurrentSkill;
	BYTE GuildTeam;
	BYTE m_byGensInfluence;
	BYTE PK;
	BYTE AttackFlag;
	BYTE AttackTime;
	BYTE Unk_34;
	BYTE Dead;
	BYTE Run;
	BYTE Gap_37;
	WORD Skill;
	BYTE Unk_40;
	BYTE byExtensionSkill;
	BYTE m_byDieType;
	BYTE Unk_43;
	BYTE JumpTime;
	BYTE TargetX;
	BYTE TargetY;
	BYTE Unk_47;
	BYTE Unk_48;
	BYTE Unk_49;
	BYTE UseSkillIndex;
	BYTE Unk_51;
	BYTE Unk_52;
	BYTE Gap_53;
	WORD Unk_54;
	BYTE Movement;
	BYTE Unk_57;
	BYTE CollisionTime;
	BYTE Gap_59;
	WORD GuildMarkIndex;
	short Key;
	WORD TargetCharacter;
	WORD ServerCodeOfHomeWorld;
	BYTE Unk_68[255];
	WORD Level;
	WORD MonsterIndex;
	WORD Unk_328;
	WORD LastDamageRecv;
	WORD MoveSpeed;
	WORD Unk_334;
	DWORD Max_HP;
	DWORD HP;
	WORD MonsterLevel;
	WORD UU_346;
	DWORD MonsterKind;
	DWORD Unk_352;
	DWORD ExtendStateTime;
	DWORD LongRangeAttack;
	DWORD Unk_364;
	DWORD Item;
	DWORD Unk_372;
	DWORD Unk_376;
	DWORD Unk_380;
	DWORD PositionX;
	DWORD PositionY;
	DWORD m_iDeleteTime;
	DWORD Unk_396;
	BYTE Unk_400;
	BYTE Gap_401[3];
	DWORD Unk_404;
	float ProtectGuildMarkWorldTime;
	float Unk_412;
	float Unk_416;
	float Unk_420;
	float Rot;
	vec3_t V3f_428;
	vec3_t Light;
	_PART_t EquipmentParts[6];
	_PART_t Weapon[2];
	_PART_t Wing;
	_PART_t Helper;
	_PART_t Flag;
	_PATH_t Path_848;
	void* m_pParts;
	BYTE Decoration;
	BYTE Gap_941[3];
	DWORD part_3;
	DWORD Unk_948;
	DWORD PetDarkSpirit;
	PET_INFO PetInfo[2];
	BYTE FenrirType;
	BYTE Gap_1005;
	WORD MuunRideIndex;
	DWORD MuunIndex[3];
	DWORD MuunSlotUse;
	DWORD part_2;
	DWORD Unk_1028;
	WORD m_CursedTempleCurSkill;
	BYTE m_CursedTempleCurSkillPacket;
	BYTE Gap_1035;
	OBJECT Object;
	BYTE Unk_1820;
	BYTE Gap_1821[3];
	DWORD Unk_1824;
	DWORD Unk_1828;
	DWORD Unk_1832;
	DWORD Unk_1836;
	VS2008::string OwnerName;
	VS2008::string CharName;
};

//======================================

#define WORLD_LIST_DATA \
	{0, "Lorencia"}, \
	{1, "Dungeon"}, \
	{2, "Devias"}, \
	{3, "Noria"}, \
	{4, "Losttower"}, \
	{6, "Stadium"}, \
	{7, "Atlans"}, \
	{8, "Tarkan"}, \
	{9, "Devil Square"}, \
	{10, "Icarus"}, \
	{11, "Blood Castle 1"}, \
	{12, "Blood Castle 2"}, \
	{13, "Blood Castle 3"}, \
	{14, "Blood Castle 4"}, \
	{15, "Blood Castle 5"}, \
	{16, "Blood Castle 6"}, \
	{17, "Blood Castle 7"}, \
	{18, "Chaos Castle 1"}, \
	{19, "Chaos Castle 2"}, \
	{20, "Chaos Castle 3"}, \
	{21, "Chaos Castle 4"}, \
	{22, "Chaos Castle 5"}, \
	{23, "Chaos Castle 6"}, \
	{24, "Kalima 1"}, \
	{25, "Kalima 2"}, \
	{26, "Kalima 3"}, \
	{27, "Kalima 4"}, \
	{28, "Kalima 5"}, \
	{29, "Kalima 6"}, \
	{30, "Valley Of Loren"}, \
	{31, "Land Of Trials"}, \
	{32, "Devil Square"}, \
	{33, "Aida"}, \
	{34, "Crywolf"}, \
	{35, "CrywolfSecondZone"}, \
	{36, "Kalima 7"}, \
	{37, "Kanturu"}, \
	{38, "Kanturu Core"}, \
	{39, "Kanturu Boss"}, \
	{39, "Kanturu Boss"}, \
	{40, "Event Zone"}, \
	{41, "Barracks"}, \
	{42, "Refuge"}, \
	{45, "Illusion Temple 1"}, \
	{46, "Illusion Temple 2"}, \
	{47, "Illusion Temple 3"}, \
	{48, "Illusion Temple 4"}, \
	{49, "Illusion Temple 5"}, \
	{50, "Illusion Temple 6"}, \
	{51, "Elbeland"}, \
	{52, "Blood Castle 8"}, \
	{53, "Chaos Castle 7"}, \
	{56, "Swamp Of Calmness"}, \
	{57, "Raklion"}, \
	{58, "Raklion Hatchery"}, \
	{62, "XMas"}, \
	{63, "Vulcanus"}, \
	{64, "Vulcanus Duel"}, \
	{69, "Imperial Fortress 1"}, \
	{70, "Imperial Fortress 2"}, \
	{71, "Imperial Fortress 3"}, \
	{72, "Imperial Fortress 4"}, \
	{79, "Loren Market"}, \
	{80, "Kalrutan 1"}, \
	{81, "Kalrutan 2"}, \
	{82, "DoubleGoer 1"}, \
	{83, "DoubleGoer 2"}, \
	{84, "DoubleGoer 3"}, \
	{85, "DoubleGoer 4"}, \
	{86, "DoubleGoer 5"}, \
	{87, "DoubleGoer 6"}, \
	{88, "DoubleGoer 7"}, \
	{89, "DoubleGoer 8"}, \
	{90, "DoubleGoer 9"}, \
	{91, "Acheron 1"}, \
	{92, "Acheron 2"}, \
	{95, "DevEnter 1"}, \
	{96, "DevEnter 2"}, \
	{97, "Chaos Castle Final"}, \
	{98, "Illusion Temple Final 1"}, \
	{99, "Illusion Temple Final 2"}, \
	{100, "Uruk Mountain"}, \
	{101, "Uruk Mountain"}, \
	{102, "Tormented Square"}, \
	{103, "Tormented Square"}, \
	{104, "Tormented Square"}, \
	{105, "Tormented Square"}, \
	{106, "Tormented Square"}, \
	{110, "Nars"}, \
	{112, "Ferea"}, \
	{113, "Nixies Lake"}, \
	{114, "The labyrinth of Dimensions"}, \
	{115, "The labyrinth of Dimensions"}, \
	{116, "Deep Dungeon 1"}, \
	{117, "Deep Dungeon 2"}, \
	{118, "Deep Dungeon 3"}, \
	{119, "Deep Dungeon 4"}, \
	{120, "Deep Dungeon 5"}, \
	{121, "Place of Qualification"}, \
	{122, "Swamp of Darkness"}, \
	{123, "Kubera Mine"}, \
	{124, "Kubera Mine"}, \
	{125, "Kubera Mine"}, \
	{126, "Kubera Mine"}, \
	{127, "Kubera Mine"}, \
	{128, "Abyss Atlans 1"}, \
	{129, "Abyss Atlans 2"}, \
	{130, "Abyss Atlans 3"}, \
	{131, "Scorched Canyon"}, \
	{132, "Scarlet Icarus"}, \
	{133, "Temple of Arnil"}

//======================================