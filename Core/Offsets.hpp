#pragma once

//GameVersion=v3.0.53.30
 
// Core    
constexpr long OFF_REGION = 0x140000000;                      //[Static]->Region
constexpr long OFF_LEVEL = 0x16d6ac0;                         //[Miscellaneous]->LevelName
constexpr long OFF_LOCAL_PLAYER = 0x21615d8;                  //[Miscellaneous]->LocalPlayer
constexpr long OFF_ENTITY_LIST = 0x1db2ed8;                   //[Miscellaneous]->cl_entitylist
 
constexpr long OFF_NAME_LIST = 0xc451fb0;                     //[Miscellaneous]->NameList
constexpr long OFF_NAME_INDEX = 0x38;                           //nameIndex (same)
 
// HUD
constexpr long OFF_VIEWRENDER = 0x73cc460;                    //[Miscellaneous]->ViewRenderer
constexpr long OFF_VIEWMATRIX = 0x11a350;                     //[Miscellaneous]->ViewMatrix
 
// Player
constexpr long OFF_HEALTH = 0x02fc;                           //[RecvTable.DT_Player]->m_iHealth
constexpr long OFF_MAXHEALTH = 0x0438;                        //[RecvTable.DT_Player]->m_iMaxHealth
constexpr long OFF_SHIELD = 0x01a0;                           //[RecvTable.DT_TitanSoul]->m_shieldHealth
constexpr long OFF_MAXSHIELD = 0x01a4;                        //[RecvTable.DT_TitanSoul]->m_shieldHealthMax
 
constexpr long OFF_INATTACK = 0x073cd740;                     //[Buttons]->in_attack
 
constexpr long OFF_CAMERAORIGIN = 0x1e90;                     //[Miscellaneous]->CPlayer!camera_origin
constexpr long OFF_STUDIOHDR = 0xfb0;                        //[Miscellaneous]->CBaseAnimating!m_pStudioHdr
constexpr long OFF_BONES = 0x0d60 + 0x48;                     //m_nForceBone
 
constexpr long OFF_LOCAL_ORIGIN = 0x017c;                     //[DataMap.C_BaseEntity]->m_vecAbsOrigin
constexpr long OFF_ABSVELOCITY = 0x0170;                      //[DataMap.C_BaseEntity]->m_vecAbsVelocity
 
constexpr long OFF_ZOOMING = 0x1b91;                          //[RecvTable.DT_Player]->m_bZooming
constexpr long OFF_TEAM_NUMBER = 0x030c;                      //[RecvTable.DT_BaseEntity]->m_iTeamNum
constexpr long OFF_NAME = 0x0449;                             //[RecvTable.DT_BaseEntity]->m_iName
constexpr long OFF_LIFE_STATE = 0x0658;                       //[RecvTable.DT_Player]->m_lifeState
constexpr long OFF_BLEEDOUT_STATE = 0x26a0;                   //[RecvTable.DT_Player]->m_bleedoutState  
constexpr long OFF_LAST_VISIBLE_TIME = 0x194d + 0x3;          //[RecvTable.DT_BaseCombatCharacter]->m_hudInfo_visibilityTestAlwaysPasses + 0x3
constexpr long OFF_LAST_AIMEDAT_TIME = 0x194d + 0x3 + 0x8;    //[RecvTable.DT_BaseCombatCharacter]->m_hudInfo_visibilityTestAlwaysPasses + 0x3 + 0x8
constexpr long OFF_VIEW_ANGLES = 0x24f4 - 0x14;               //[DataMap.C_Player]-> m_ammoPoolCapacity - 0x14
constexpr long OFF_PUNCH_ANGLES = 0x23f8;                     //[DataMap.C_Player]->m_currentFrameLocalPlayer.m_vecPunchWeapon_Angle
constexpr long OFF_YAW = 0x21fc - 0x8;                        //m_currentFramePlayer.m_ammoPoolCount - 0x8
 
// Weapon 
constexpr long OFF_WEAPON_HANDLE = 0x18f4;                    //[RecvTable.DT_Player]->m_latestPrimaryWeapons
constexpr long OFF_WEAPON_INDEX = 0x1738;                     //[RecvTable.DT_WeaponX]->m_weaponNameIndex
constexpr long OFF_PROJECTILESCALE = 0x04ec + 0x1978;         //projectile_gravity_scale + [WeaponSettingsMeta]base
constexpr long OFF_PROJECTILESPEED = 0x04e4 + 0x1978;         //projectile_launch_speed + [WeaponSettingsMeta]base
constexpr long OFF_OFFHAND_WEAPON = 0x1904;                   //m_latestNonOffhandWeapons
constexpr long OFF_CURRENTZOOMFOV = 0x1590 + 0x00b8;          //m_playerData + m_curZoomFOV
constexpr long OFF_TARGETZOOMFOV = 0x1590 + 0x00bc;           //m_playerData + m_targetZoomFOV

// Glow
constexpr long OFF_GLOW_HIGHLIGHTS = 0xB7A6050;               //HighlightSettings
constexpr long OFF_GLOW_HIGHLIGHTS_MAX = 0xB7A6068;

constexpr long OFF_TIME_BASE = 0x20b8 ; //m_currentFramePlayer.timeBase  
// PREVIOUS VERSION

// Core
constexpr long OFF_MODELNAME = 0x0030;                        //m_ModelName

// GLOW
constexpr long HIGHLIGHT_TYPE_SIZE = 0x28;  
constexpr long OFF_GLOW_ENABLE = 0x294;                       //Script_Highlight_GetCurrentContext
constexpr long OFF_GLOW_THROUGH_WALL = 0x278;                 //Script_Highlight_SetVisibilityType
constexpr long OFF_GLOW_FIX = 0x270;
constexpr long OFF_GLOW_HIGHLIGHT_ID = 0x298;                 //[DT_HighlightSettings].m_highlightServerActiveStates    


constexpr long OFF_IN_JUMP = 0x073cd828;                       //[Buttons]->in_jump
constexpr long OFF_IN_DUCK = 0x073cc720;                       //[Buttons]->in_duck
constexpr long OFF_IN_FORWARD = 0x073cc470;                       //[Buttons]->in_forward
constexpr long OFF_TRAVERSAL_START_TIME = 0x2ac0;             //[Buttons]->m_traversalStartTime
constexpr long OFF_TRAVERSAL_PROGRESS = 0x2abc;             //[Buttons]->m_traversalProgress
constexpr long OFF_WALL_RUN_START_TIME = 0x3524;           //[Buttons]->m_wallRunStartTime
constexpr long OFF_WALL_RUN_CLEAR_TIME = 0x3528;             //[Buttons]->m_wallRunClearTime

#define OFFSET_GRAPPLE 0x2c78 //[RecvTable.DT_Player].m_grapple
#define OFFSET_GRAPPLE_ATTACHED 0x0048//m_grappleAttached
