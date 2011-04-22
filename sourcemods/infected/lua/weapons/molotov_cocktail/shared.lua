

if ( SERVER ) then

	AddCSLuaFile( "shared.lua" )
	
	SWEP.HoldType			= "grenade"
	
end

if ( CLIENT ) then

	SWEP.PrintName			= "Molotov Cocktail" -- By SmiteTheHero
	SWEP.Author				= "SmiteTheHero"
	SWEP.Slot				= 5
	SWEP.Slotpos                            = 1
        SWEP.DrawAmmo 			= false
	SWEP.DrawCrosshair 		= true
	SWEP.SlotPos			= 3
    SWEP.Contact 			= "smitethehero@hotmail.co.uk"
    SWEP.Purpose 			= "Starts Fires"
    SWEP.Instructions 		= "Left click to Throw, Right click to Roll"
		
end

SWEP.ViewModel			= "models/weapons/v_molotov.mdl"
SWEP.WorldModel			= "models/weapons/w_molotov.mdl"
SWEP.ViewModelFOV   	= 64
SWEP.Category			= "Explosives"

SWEP.Weight				= 2
SWEP.AutoSwitchTo		= false
SWEP.AutoSwitchFrom		= false

SWEP.Spawnable			= true
SWEP.AdminSpawnable		= false

SWEP.Primary.Recoil			= 0
SWEP.Primary.Delay 			= 0
SWEP.Primary.Damage			= 0
SWEP.Primary.ClipSize		= -1
SWEP.Primary.Reload 		= 0
SWEP.Primary.Automatic		= false
SWEP.Primary.Ammo			= "none"

SWEP.Secondary.Delay		= 0
SWEP.Secondary.ClipSize		= -1
SWEP.Secondary.DefaultClip	= -1
SWEP.Secondary.Automatic	= false
SWEP.Secondary.Ammo			= "none"


/*-----DO NOT DO ANYTHING UNDER THIS LINE EXCEPTING YOU KNOW WHAT YOU ARE DOING!!! -----*/	

function SWEP:Initialize()
util.PrecacheSound( "WeaponFrag.Throw" )
util.PrecacheModel( "models/props_junk/garbage_glassbottle003a.mdl" )
end

function SWEP:Think()
end

function SWEP:PrimaryAttack()
local Player = self.Owner

local Molotov = ents.Create( "sent_molotov" )
	Molotov:SetOwner( Player )
	Molotov:SetPos( Player:GetShootPos() )
--	Molotov:SetAngel( Player:GetAimVector() )
	Molotov:Spawn()

	local mPhys = Molotov:GetPhysicsObject()
	local Force = Player:GetAimVector() * 2500
		
		mPhys:ApplyForceCenter( Force )
		
	self.Weapon:EmitSound( "WeaponFrag.Throw" )
	self.Weapon:SendWeaponAnim( ACT_VM_THROW )
	timer.Simple( 0.3, self.Weapon.SendWeaponAnim, self, ACT_VM_IDLE )
	self.Weapon:SetNextPrimaryFire( CurTime() + 2 )
	self.Weapon:SetNextSecondaryFire( CurTime() + 2 )
end

function SWEP:SecondaryAttack()
local Player = self.Owner

local Molotov = ents.Create( "sent_molotov_timed" )
	Molotov:SetOwner( Player )
	Molotov:SetPos( Player:GetShootPos() )
--	Molotov:SetAngel( Player:GetAimVector() )
	Molotov:Spawn()

	local mPhys = Molotov:GetPhysicsObject()
	local Force = Player:GetAimVector() * 500
		
		mPhys:ApplyForceCenter( Force )
		
	self.Weapon:EmitSound( "WeaponFrag.Throw" )
	self.Weapon:SendWeaponAnim( ACT_VM_THROW )
	timer.Simple( 0.3, self.Weapon.SendWeaponAnim, self, ACT_VM_IDLE )
	self.Weapon:SetNextSecondaryFire( CurTime() + 2 )
	self.Weapon:SetNextPrimaryFire( CurTime() + 2 )
end

function SWEP:Deploy()
	self.Weapon:SendWeaponAnim( ACT_VM_DRAW )
end

	
function SWEP:Reload()
	return false
end
