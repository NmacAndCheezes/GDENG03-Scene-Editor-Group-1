#include "MenuTab.h"
#include <EditorStates/EditorBackend.h>
#include <GameEngine/Debug.h>
#include <EditorStates/EditorActions/EditorAction.h>
#include <EditorStates/EditorActions/EditorActionHistory.h>
#include <GameEngine/Managers/PhysicsEngine.h>
#include <EditorGUI/HierarchyTab.h>
#include <EditorGUI/EditorGUIManager.h>

#include "WindowSystem/FileDialogs.h"
#include "GameEngine/Managers/SceneManager.h"
#include <GameEngine/Managers/GameObjectManager.h>
#include <GameEngine/GameObjects/Primitives/CubeObject.h>
#include <GameEngine/GameObjects/Primitives/SphereObject.h>
#include <GameEngine/GameObjects/ModelObject.h>
#include <GameEngine/GameObjects/PhysicsObject.h>
#include <GameEngine/MathUtils.h>
#include <GameEngine/GameObjects/Primitives/CylinderObject.h>
#include <GameEngine/GameObjects/Primitives/ConeObject.h>



MenuTab::MenuTab() : AUITab(EditorGUIManager::TabNames::MENU_TAB.data()) 
{

}

MenuTab::~MenuTab()
{

}

void MenuTab::RenderUI()
{
	if (ImGui::BeginMainMenuBar())
	{
        RenderFilesMenu();
        RenderTabsMenu();
        RenderToolsMenu();
        RenderUndoRedoMenu();
        RenderAboutMenu();
        RenderSceneStatesMenu();

		ImGui::EndMainMenuBar();
	}
         
}

void MenuTab::RenderFilesMenu()
{
    if (ImGui::BeginMenu("Files"))
    {
        if (ImGui::MenuItem("New"))
        {

        }

        if (ImGui::MenuItem("Open", "Ctrl+O"))
        {
            const char* filters =
                "Level (*.level)\0*.level\0"
                ;
            std::string scenePath = FileDialogs::OpenFile(filters); 
            SceneManager::GetInstance()->OpenScene(scenePath);
        }

        if (ImGui::MenuItem("Save", "Ctrl+S"))
        {
            std::string scenePath = "";
            if (SceneManager::GetInstance()->GetActiveSceneName() == "")
            {
                const char* filters =
                    "Level (*.level)\0*.level\0"
                    ;
                scenePath = FileDialogs::SaveFile(filters);
            }
            SceneManager::GetInstance()->SaveScene(scenePath);
        }

        if (ImGui::MenuItem("Save As.."))
        {
            const char* filters =
                "Level (*.level)\0*.level\0"
                ;
            std::string scenePath = FileDialogs::SaveFile(filters);
            SceneManager::GetInstance()->SaveScene(scenePath); 
        }

        ImGui::EndMenu();
    }
}

void MenuTab::RenderTabsMenu()
{
    if (ImGui::BeginMenu("Tabs"))
    {
        if (ImGui::MenuItem("Hierarchy"))
        {
            AUITab* hierarchy = EditorGUIManager::GetInstance()->GetTab(EditorGUIManager::TabNames::HIERARCHY_TAB.data());
            if (hierarchy) hierarchy->Enabled = true;
        }

        if (ImGui::MenuItem("Inspector"))
        {
            AUITab* inspector = EditorGUIManager::GetInstance()->GetTab(EditorGUIManager::TabNames::INSPECTOR_TAB.data());
            if (inspector) inspector->Enabled = true;
        }

        if (ImGui::MenuItem("Console"))
        {
            AUITab* console = EditorGUIManager::GetInstance()->GetTab(EditorGUIManager::TabNames::CONSOLE_TAB.data());
            if (console) console->Enabled = true;
        }


        ImGui::EndMenu();
    }
}

void MenuTab::RenderToolsMenu()
{
    if (ImGui::BeginMenu("Tools"))
    {
        if (ImGui::MenuItem("Color Picker"))
        {
            AUITab* colorPicker = EditorGUIManager::GetInstance()->GetTab(EditorGUIManager::TabNames::COLOR_PICKER_TAB.data());
            if (colorPicker) colorPicker->Enabled = true;
        }

        ImGui::EndMenu(); 
    }
}

void MenuTab::RenderAboutMenu()
{
    if (ImGui::BeginMenu("About")) 
    {
        if (ImGui::MenuItem("Credits")) 
        {
            AUITab* about = EditorGUIManager::GetInstance()->GetTab(EditorGUIManager::TabNames::CREDITS_TAB.data()); 
            if (about) about->Enabled = true; 
        }

        ImGui::EndMenu(); 
    }
}

void MenuTab::RenderSceneStatesMenu()
{
    if (ImGui::BeginMenu("Play/Pause"))
    {
        if (ImGui::MenuItem("Play") && EditorBackend::get()->getState() != EditorBackend::PLAY)
        {
            Debug::Log("Playing game");
            //PhysicsEngine::GetInstance()->Init();
            
            EditorActionHistory::get()->RecordEditStates();
            EditorBackend::get()->setState(EditorBackend::PLAY);
        }
        else if (ImGui::MenuItem("Pause") && EditorBackend::get()->getState() != EditorBackend::PAUSE)
        {
            Debug::Log("Pausing game");
            EditorBackend::get()->setState(EditorBackend::PAUSE);
        }
        else if (ImGui::MenuItem("Stop") && EditorBackend::get()->getState() != EditorBackend::EDIT)
        {
            Debug::Log("Returning to edit mode");
            EditorBackend::get()->setState(EditorBackend::EDIT);
            EditorActionHistory::get()->SetToEditState();
            //PhysicsEngine::GetInstance()->Reset();
            //PhysicsEngine::GetInstance()->Release();
            // no functionality for the HO it will be implemented at the Scene Editor MO
        }

        ImGui::EndMenu();
    }

    if (ImGui::Button("Frame step"))
    {
        EditorBackend::get()->startFrameStep();
    }
}

void MenuTab::RenderUndoRedoMenu()
{
    if (EditorBackend::get()->getState() != EditorBackend::EDIT) return;

    if (ImGui::BeginMenu("GameObject"))
    {
        if (ImGui::MenuItem("Spawn Cube"))
        {
            CubeObject* cube = new CubeObject("cube", true);
            cube->GetTransform()->Position = Vector3(0.f, 0.f, 0.f);
            GameObjectManager::GetInstance()->AddRootObject(cube);
        }
        if (ImGui::MenuItem("Spawn Sphere"))
        {
            SphereObject* cube = new SphereObject("sphere", true);
            cube->GetTransform()->Position = Vector3(0.f, 0.f, 0.f);
            GameObjectManager::GetInstance()->AddRootObject(cube);
        }
        if (ImGui::MenuItem("Spawn Cylinder"))
        {
            CylinderObject* cube = new CylinderObject("cylinder", true);
            cube->GetTransform()->Position = Vector3(0.f, 0.f, 0.f);
            GameObjectManager::GetInstance()->AddRootObject(cube);
        }
        if (ImGui::MenuItem("Spawn Cone"))
        {
            ConeObject* cube = new ConeObject("cone", true);
            cube->GetTransform()->Position = Vector3(0.f, 0.f, 0.f);
            GameObjectManager::GetInstance()->AddRootObject(cube);
        }
        if (ImGui::MenuItem("Spawn Teapot"))
        {
            ModelObject* model = new ModelObject("teapot3.obj", "brick.png");
            model->GetTransform()->ScaleUniformly(0.35f);
            GameObjectManager::GetInstance()->AddRootObject(model);
        }
        if (ImGui::MenuItem("Spawn 100RB Cubes"))
        {
            for (int i = 0; i < 100; i++)
            {
                PhysicsObject* phy1 = new PhysicsObject(EPrimitiveMeshTypes::Cube);
                phy1->GetTransform()->Position = Vector3(
                    MathUtils::RandFloatWithRange(-5.f, 5.f),
                    30.f,
                    MathUtils::RandFloatWithRange(-5.f, 5.f)
                );
                GameObjectManager::GetInstance()->AddRootObject(phy1);
                phy1->GetRB()->BodyType = rp3d::BodyType::DYNAMIC;
            }
        }
        if (ImGui::MenuItem("Spawn P6 Plane"))
        {
            PhysicsObject* phy1 = new PhysicsObject(EPrimitiveMeshTypes::Plane);
            phy1->GetTransform()->Position = Vector3(0.0f, 0.0f, 0.0f);
            phy1->GetTransform()->LocalScale = Vector3(7.0f, 1.0f, 7.0f);
            GameObjectManager::GetInstance()->AddRootObject(phy1);
            phy1->GetRB()->BodyType = rp3d::BodyType::STATIC;
        }
        ImGui::EndMenu();
    }
}
