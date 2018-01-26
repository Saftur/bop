#include "stdafx.h"
#include <AEEngine.h>
#include "Mesh.h"
#include "PlatformManager.h"

vector<PlatformManager::Platform> PlatformManager::platforms;
AEGfxVertexList *PlatformManager::platformMesh;
Sprite *PlatformManager::platformSprite;

//float PlatformManager::scroll = 0;

void PlatformManager::Init()
{
	AEGfxVertexList* verts;

	// Start adding vertices
	AEGfxMeshStart();

	// Add some triangles
	AEGfxTriAdd(-0.5f, 0.5f, 0xFF000000, 0.0f, 0.0f,
		0.5f, 0.5f, 0xFF000000, 1, 0.0f,
		-0.5f, -0.5f, 0xFF000000, 0.0f, 1);
	AEGfxTriAdd(0.5f, -0.5f, 0xFF000000, 1, 1,
		0.5f, 0.5f, 0xFF000000, 1, 0.0f,
		-0.5f, -0.5f, 0xFF000000, 0.0f, 1);

	// Stop adding vertices
	verts = AEGfxMeshEnd();
	platformMesh = verts;//MeshCreateQuad(0.5f, 0.5f, 1, 1, "Platform Mesh");
	platformSprite = new Sprite("Platform Sprite");
	platformSprite->SetMesh(platformMesh);
}

void PlatformManager::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);
	Vector2D st = Transform::GetCamTranslation();
	Vector2D scrMin = { AEGfxGetWinMinX() - st.X(), AEGfxGetWinMinY() - st.Y() };
	Vector2D scrMax = { AEGfxGetWinMaxX() - st.X(), AEGfxGetWinMaxY() - st.Y() };
	Transform *pt;
	Vector2D ptrs;
	Vector2D pscl;
	Transform::SetCamIsDirty(true);
	for (Platform &p : platforms) {
		pt = &p.transform;
		ptrs = pt->GetTranslation();
		pscl = pt->GetScale();
		if (p.dir != Vector2D(0,0)) {
			ptrs += p.dir * p.moveSpeed * dt;
			Vector2D diff = p.path[p.pn] - ptrs;
			if (diff.X() * -p.dir.X() >= 0 && diff.Y() * -p.dir.Y() >= 0) {
				ptrs = p.path[p.pn];
				if (++p.pn >= p.path.size())
					p.pn = 0;
				p.dir = { 0,0 };
			}
			pt->SetTranslation(ptrs);
		}
		if (p.dir == Vector2D(0,0) && p.moveSpeed > 0) {
			p.dir = (p.path[p.pn] - ptrs).Normalized();
		}
		if (ptrs.X() + pscl.X() / 2 > scrMin.X() && ptrs.X() - pscl.X() / 2 < scrMax.X() &&
			ptrs.Y() + pscl.Y() / 2 > scrMin.Y() && ptrs.Y() - pscl.Y() / 2 < scrMax.Y()) {
			platformSprite->Draw(*pt);
		}
	}
	Transform::SetCamIsDirty(false);
}

void PlatformManager::Shutdown()
{
	platforms.clear();
	AEGfxMeshFree(platformMesh);
	delete platformSprite;
}

void PlatformManager::AddPlatform(Transform transform, float jump, bool trampoline, float moveSpeed, vector<Vector2D> path, bool killsInTime)
{
	path.push_back(transform.GetTranslation());
	platforms.push_back({ transform, jump, trampoline, moveSpeed, path, killsInTime, 0, {0, 0} });
}

PlatformManager::Platform* PlatformManager::IsOnPlatform(GameObject * object, Vector2D * groundPosition)
{
	Transform *ot = object->GetTransform();
	if (!ot) return nullptr;
	Vector2D otrs = ot->GetTranslation();
	Vector2D oscl = ot->GetScale();
	Transform *pt;
	Vector2D ptrs;
	Vector2D pscl;
	for (Platform &p : platforms) {
		pt = &p.transform;
		ptrs = pt->GetTranslation();
		pscl = pt->GetScale();
		if ((otrs.X() - oscl.X() / 4 > ptrs.X() - pscl.X() / 2 + 1 && otrs.X() - oscl.X() / 4 < ptrs.X() + pscl.X() / 2 - 1 ||
			otrs.X() + oscl.X() / 4 > ptrs.X() - pscl.X() / 2 + 1 && otrs.X() + oscl.X() / 4 < ptrs.X() + pscl.X() / 2 - 1) &&
			otrs.Y() - oscl.Y() / 2 > ptrs.Y() - pscl.Y() / 2 && otrs.Y() - oscl.Y() / 2 < ptrs.Y() + pscl.Y() / 2 + 1) {
			if (groundPosition) {
				groundPosition->X(otrs.X());
				groundPosition->Y(ptrs.Y() + pscl.Y() / 2 + oscl.Y() / 2);
			}
			return &p;
		}
	}
	return nullptr;
}

int PlatformManager::HittingSide(GameObject * object, Vector2D * newPosition, Vector2D * newVelocity)
{
	Transform *ot = object->GetTransform();
	if (!ot) return 0;
	Vector2D otrs = ot->GetTranslation();
	Vector2D oscl = ot->GetScale();
	Transform *pt;
	Vector2D ptrs;
	Vector2D pscl;
	for (Platform &p : platforms) {
		pt = &p.transform;
		ptrs = pt->GetTranslation();
		pscl = pt->GetScale();
		if ((otrs.X() - oscl.X() / 4 > ptrs.X() - pscl.X() / 2 + 1 && otrs.X() - oscl.X() / 4 < ptrs.X() + pscl.X() / 2 - 1 ||
			otrs.X() + oscl.X() / 4 > ptrs.X() - pscl.X() / 2 + 1 && otrs.X() + oscl.X() / 4 < ptrs.X() + pscl.X() / 2 - 1 ||
			otrs.X() > ptrs.X() - pscl.X() / 2 + 1 && otrs.X() < ptrs.X() + pscl.X() / 2 - 1) &&
			otrs.Y() + oscl.Y() / 2 > ptrs.Y() - pscl.Y() / 2 - 1 && otrs.Y() + oscl.Y() / 2 < ptrs.Y() + pscl.Y() / 2) {
			if (newPosition) {
				newPosition->X(otrs.X());
				newPosition->Y(ptrs.Y() - pscl.Y() / 2 - oscl.Y() / 2 - 1);
			}
			if (newVelocity) {
				newVelocity->X(object->GetPhysics()->GetVelocity().X());
				newVelocity->Y(0);
			}
			return 2;
		}
		if ((otrs.Y() - oscl.Y() / 4 > ptrs.Y() - pscl.Y() / 2 + 1 && otrs.Y() - oscl.Y() / 4 < ptrs.Y() + pscl.Y() / 2 - 1 ||
			otrs.Y() + oscl.Y() / 4 > ptrs.Y() - pscl.Y() / 2 + 1 && otrs.Y() + oscl.Y() / 4 < ptrs.Y() + pscl.Y() / 2 - 1 ||
			otrs.Y() > ptrs.Y() - pscl.Y() / 2 + 1 && otrs.Y() < ptrs.Y() + pscl.Y() / 2 - 1) &&
			(otrs.X() - oscl.X() / 2 > ptrs.X() - pscl.X() / 2 && otrs.X() - oscl.X() / 2 < ptrs.X() + pscl.X() / 2 + 1 ||
			otrs.X() + oscl.X() / 2 > ptrs.X() - pscl.X() / 2 - 1 && otrs.X() + oscl.X() / 2 < ptrs.X() + pscl.X() / 2)) {
			if (newPosition) {
				if (otrs.X() < ptrs.X())
					newPosition->X(ptrs.X() - pscl.X() / 2 - oscl.X() / 2);
				else
					newPosition->X(ptrs.X() + pscl.X() / 2 + oscl.X() / 2);
				newPosition->Y(otrs.Y());
			}
			if (newVelocity) {
				newVelocity->X(0);
				newVelocity->Y(object->GetPhysics()->GetVelocity().Y());
			}
			if (otrs.X() < ptrs.X())
				return 3;
			else return 1;
		}
	}
	return 0;
}
