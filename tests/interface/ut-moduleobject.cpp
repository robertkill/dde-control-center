#include "gtest/gtest.h"

#include "interface/moduleobject.h"

#include <QDebug>

DCC_USE_NAMESPACE

class Tst_ModuleObject : public testing::Test
{
public:
    void SetUp() override
    {
        obj = new ModuleObject;
    }
    void TearDown() override
    {
        delete obj;
        obj = nullptr;
    }
public:
    ModuleObject *obj = nullptr;
};


TEST_F(Tst_ModuleObject, coverage)
{
    obj->setName("Name");
    obj->setDisplayName("DisplayName");
    obj->setDescription("Description");
    obj->setIcon(QIcon::fromTheme("preferences-system"));
    obj->setChildType(ModuleObject::HList);

    for (int i = 1; i < 4; i++) {
        ModuleObject *module = new ModuleObject(QString("menu%1").arg(i), QString("菜单%1").arg(i), QString("我是菜单%1").arg(i));
        module->setChildType(ModuleObject::Page);

        obj->appendChild(module);
    }

    ModuleObject *module = new ModuleObject;
    module->setName("action1");
    module->setDisplayName("选项1");
    module->setDescription("我是选项1");

    obj->childrens().last()->insertChild(1, module);

    ASSERT_TRUE(obj->findChild(module) == 2);

    obj->removeChild(0);

    ASSERT_TRUE(obj->childrens().size() == 2);

}