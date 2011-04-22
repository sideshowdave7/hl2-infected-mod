

if ( SERVER ) then

	AddCSLuaFile( "shared.lua" )
	
	SWEP.HoldType			= "grenade"
	
end

if ( CLIENT ) then

	SWEP.PrintName			= "Molotov"
	SWEP.Author				= "Zinze[RPB]"
	SWEP.Slot				= 4
	SWEP.DrawAmmo 			= false
	SWEP.DrawCrosshair 		= true
	SWEP.SlotPos			= 3
    SWEP.Contact 			= "www.ZinzesRPServer.webs.com"
    SWEP.Purpose 			= "Used to blow things up"
    SWEP.Instructions 		= "Left click to throw it awey"
		
end

SWEP.ViewModel      = "models/weapons/v_punch.mdl"
SWEP.WorldModel   = "models/weapons/w_fists_t.mdl"
SWEP.ViewModlelFOV   	= 90


SWEP.Weight				= 2
SWEP.AutoSwitchTo		= false
SWEP.AutoSwitchFrom		= false

SWEP.Spawnable			= true
SWEP.AdminSpawnable		= false

SWEP.Primary.Recoil			= 0
SWEP.Primary.Delay 			= 0
SWEP.Primary.Damage			= 0
SWEP.Primary.ClipSize		= 1
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
	if ( !self:CanPrimaryAttack() ) then return end
	self:TakePrimaryAmmo( 1 )
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
	return false
	/*self:TakeSecondaryAmmo( 1 )
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
	self.Weapon:SetNextPrimaryFire( CurTime() + 2 )*/
end

function SWEP:Deploy()
	self.Weapon:SendWeaponAnim( ACT_VM_DRAW )
end

	
function SWEP:Reload()
	return false
end
