#include "EscapeMouse.h"
#include "Actor/Survivor.h"
#include "Core/Input.h"
#include "Game/Game.h"
#include "Manager/MapManager.h"
#include "Render/Renderer.h"
#include "Util/Util.h"

EscapeMouse::EscapeMouse(const Vector2& position, std::vector<class Survivor*>* allSurvivors)
	: super(position), allSurvivors(allSurvivors)
{
}

EscapeMouse::~EscapeMouse()
{
	// 실제 객체는 레벨에서 관리
	allSurvivors = nullptr;
}

void EscapeMouse::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// Todo: 임시 코드
	// 엔터 입력시 맵 토글
	if (Input::Get().GetKeyDown(VK_RETURN))
	{
		SelectPositionClear();

		Game::Get().ToggleMenu(1);
	}

	// 마우스 좌클릭을 뗐을 때
	if (Input::Get().GetMouseButtonUp(0))
	{
		// 이전에 선택한 생존자 초기화
		std::vector<Survivor*>().swap(selectedSurvivor);

		// 포인터 예외처리
		if (allSurvivors)
		{
			// 선택 영역을 확인해서 생존자 배열에 추가
			for (Survivor* survivor : *allSurvivors)
			{
				// 생존자의 현재 위치
				Vector2 survivorPos = survivor->GetPosition();

				// 선택 영역에 생존자 위치가 포함되어 있는지 확인
				for (const Vector2& selPos : selectedPositionInConsole)
				{
					if (survivorPos == selPos)
					{
						// 영역 안에 있으면 선택된 생존자 목록에 추가
						selectedSurvivor.emplace_back(survivor);
						break;
					}
				}
			}
		}

		// 선택 영역 해제
		SelectPositionClear();

		// 선택된 배열에 접근 불가능한 생존자 삭제
		EraseUnaccessableSurvivor();
	}

	// 우클릭 시 선택한 생존자들 이동
	else if (Input::Get().GetMouseButtonDown(1))
	{
		// 선택된 생존자가 없으면 반환
		if (selectedSurvivor.empty())
		{
			return;
		}

		Vector2 targetPos = Input::Get().MousePosition();

		// 선택된 배열에 접근 불가능한 생존자 삭제
		EraseUnaccessableSurvivor();

		// 배열 셔플 하여 랜덤 이동순서
		Util::ShuffleVector(selectedSurvivor);
		 
		// 선택된 모든 생존자에게 목표 위치 전달 및 이동
		for (Survivor* survivior : selectedSurvivor)
		{
			survivior->CommandMoveTo(targetPos);
		}
	}
}

void EscapeMouse::Draw()
{
	super::Draw();

	for (const Vector2& selPos : selectedPositionInConsole)
	{
		// 선택 영역의 빈 공간인 곳만 색칠
		if (MapManager::Get().GetMapPositionData(selPos) == ' ')
		{
			Renderer::Get().Submit(" ", selPos, Color::BackGoundWhite, sortingOrder + 1);
		}
	}

	// 위치 설정
	int offsetX = MapManager::Get().GetMapWidth() + 5;
	int offsetY = MapManager::Get().GetMapHeight() - 10;
	Vector2 drawPos = Vector2(offsetX, offsetY);

	if (selectedSurvivor.empty())
	{
		Renderer::Get().Submit("선택된 생존자 없음.", drawPos, color, sortingOrder + 1);
		return;
	}

	// 선택된 배열에 접근 불가능한 생존자 삭제
	EraseUnaccessableSurvivor();

	static char buffer[256] = {};
	sprintf_s(buffer, 256, "선택된 생존자 수 : %d명", static_cast<int>(selectedSurvivor.size()));
	Renderer::Get().Submit(buffer, drawPos);

	static char infoBuffer[10][128];
	for (int i = 0;i < static_cast<int>(selectedSurvivor.size());i++)
	{
		// 혹시 모르는 예외 처리
		if (i >= 10)
		{
			break;
		}

		drawPos.y += 1;

		Survivor* survivor = selectedSurvivor[i];

		sprintf_s(infoBuffer[i], 128, "[S] 위치 : (%d, %d)",
			survivor->GetPosition().x, survivor->GetPosition().y);

		Renderer::Get().Submit(infoBuffer[i], drawPos, survivor->GetColor());
	}
}

void EscapeMouse::EraseUnaccessableSurvivor()
{
	// 배열이 비었으면 
	if (selectedSurvivor.empty())
	{
		return;
	}

	for (auto iter = selectedSurvivor.begin(); iter != selectedSurvivor.end();)
	{
		Survivor* survivor = *iter;

		// 포인터 널 체크
		if (survivor)
		{
			// 생존자가 삭제 요청을 받거나 활성화되지 않았다면 배열에서 삭제
			if (survivor->DestroyRequested() || !survivor->IsActive())
			{
				iter = selectedSurvivor.erase(iter);
			}
			else
			{
				iter++;
			}
		}
		else
		{
			iter = selectedSurvivor.erase(iter);
		}
	}
}
