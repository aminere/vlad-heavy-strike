
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := vlad
LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv1_CM
LOCAL_STATIC_LIBRARIES := android_native_app_glue shoot
LOCAL_CFLAGS := -DGL_GLEXT_PROTOTYPES=1

ifeq ($(SHOOT_PROFILE), 1)
LOCAL_CFLAGS += -D_PROFILE_
endif

ifeq ($(SHOOT_RETAIL), 1)
LOCAL_CFLAGS += -D_RETAIL_
endif

ifeq ($(VLAD_DEMO), 1)
LOCAL_CFLAGS += -D_VLAD_DEMO_
endif

LOCAL_SRC_FILES := \
main.cpp \
../../../src/game/Actor.cpp \
../../../src/game/AIManager.cpp \
../../../src/game/AreaSpawner.cpp \
../../../src/game/Asteroid.cpp \
../../../src/game/BasicEnemy.cpp \
../../../src/game/BasicFighter.cpp \
../../../src/game/BasicShooter.cpp \
../../../src/game/BasicTower.cpp \
../../../src/game/Boss1.cpp \
../../../src/game/Boss1FSM.cpp \
../../../src/game/Boss2.cpp \
../../../src/game/Boss2FSM.cpp \
../../../src/game/Boss3.cpp \
../../../src/game/Boss3FSM.cpp \
../../../src/game/Boss4.cpp \
../../../src/game/Boss4FSM.cpp \
../../../src/game/BulletManager.cpp \
../../../src/game/BulletManagerCollisionComponent.cpp \
../../../src/game/ConfigPoint.cpp \
../../../src/game/ConversationManager.cpp \
../../../src/game/ConversationVisitor.cpp \
../../../src/game/DefaultEntityRegistration.cpp \
../../../src/game/FallingAsteroid.cpp \
../../../src/game/GameData.cpp \
../../../src/game/GameEngine.cpp \
../../../src/game/GameManager.cpp \
../../../src/game/AudioManager.cpp \
../../../src/game/UIContext.cpp \
../../../src/game/PlayerList.cpp \
../../../src/game/Item.cpp \
../../../src/game/ItemManager.cpp \
../../../src/game/LaserManager.cpp \
../../../src/game/LaserManagerCollisionComponent.cpp \
../../../src/game/PelletManager.cpp \
../../../src/game/Player.cpp \
../../../src/game/PlayerCamera.cpp \
../../../src/game/PlayerCannons.cpp \
../../../src/game/PlayerCollisionComponent.cpp \
../../../src/game/PlayerFSM.cpp \
../../../src/game/PulseManager.cpp \
../../../src/game/SFXManager.cpp \
../../../src/game/SpinShooter.cpp \
../../../src/game/Kamikaze.cpp \
../../../src/game/RadialShooter.cpp \
../../../src/game/Starfield.cpp \
../../../src/game/TightPassage.cpp \
../../../src/game/TriggerSpawner.cpp \
../../../src/game/WeaponConfig.cpp \
../../../src/game/RadialTower.cpp \
../../../src/game/RadialTank.cpp \
../../../src/game/SpinningLasers.cpp \
../../../src/game/Squad.cpp \
../../../src/game/LaserSwiper.cpp \
../../../src/game/Bomber.cpp \
../../../src/game/ScoreoidUtils.cpp \
../../../src/game/KamikazeTower.cpp \
../../../src/game/DamageComponent.cpp \
../../../src/game/UIHandler.cpp \
../../../src/game/VKButton.cpp \
../../../src/game/VKeyboard.cpp

include $(BUILD_SHARED_LIBRARY)

include $(LOCAL_PATH)/../../../../shoot/build/android/Android.mk


