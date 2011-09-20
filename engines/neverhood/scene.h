/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef NEVERHOOD_SCENE_H
#define NEVERHOOD_SCENE_H

#include "common/array.h"
#include "neverhood/neverhood.h"
#include "neverhood/background.h"
#include "neverhood/entity.h"
#include "neverhood/graphics.h"
#include "neverhood/klayman.h"
#include "neverhood/module.h"
#include "neverhood/palette.h"
#include "neverhood/smackerplayer.h"
#include "neverhood/sprite.h"
#include "neverhood/staticdata.h"

namespace Neverhood {

#define InsertKlayman(KlaymanClass, X, Y) _klayman = new KlaymanClass(_vm, this, X, Y); addSprite(_klayman)
#define InsertKlaymanInitArgs(KlaymanClass, X, Y, InitArgs) _klayman = new KlaymanClass(_vm, this, X, Y); ((KlaymanClass*)_klayman)->init InitArgs; addSprite(_klayman)

#define InsertSprite(SpriteClass) (SpriteClass*)addSprite(new SpriteClass(_vm)); 
#define InsertSpriteInitArgs(SpriteClass, InitArgs) (SpriteClass*)addSprite((new SpriteClass(_vm))-> init InitArgs);

class Scene : public Entity {
public:
	Scene(NeverhoodEngine *vm, Module *parentModule, bool clearHitRects);
	virtual ~Scene();
	virtual void draw();
	void addEntity(Entity *entity);
	bool removeEntity(Entity *entity);
	void addSurface(BaseSurface *surface);
	bool removeSurface(BaseSurface *surface);
	Sprite *addSprite(Sprite *sprite);
	void setSurfacePriority(BaseSurface *surface, int priority);
	void deleteSprite(Sprite **sprite);
	Background *addBackground(Background *background);
	void setBackground(uint32 fileHash, bool dirtyBackground = true);
	void changeBackground(uint32 fileHash);
	void setBackgroundY(int16 y) { _background->getSurface()->getDrawRect().y = y; }
	int16 getBackgroundY() { return _background->getSurface()->getDrawRect().y; }
	void setPalette(uint32 fileHash = 0);
	Sprite *insertStaticSprite(uint32 fileHash, int surfacePriority);
	void insertMouse433(uint32 fileHash, NRect *mouseRect = NULL);
	void insertMouse435(uint32 fileHash, int16 x1, int16 x2);
	void insertNavigationMouse(uint32 fileHash, int type);
	void showMouse(bool visible);
	void changeMouseCursor(uint32 fileHash);
	SmackerPlayer *addSmackerPlayer(SmackerPlayer *smackerPlayer);
	void update();
protected:
	Module *_parentModule;
	Common::Array<Entity*> _entities;
	Common::Array<BaseSurface*> _surfaces;
	bool _systemCallbackFlag;
	MessageList *_messageList;
	uint _messageListCount;
	uint _messageListIndex;
	bool _messageListFlag1;
	NPoint _mouseClickPos;
	bool _mouseClicked;
	DataResource _dataResource;
	RectList *_rectList;
	HitRectList _hitRectList;
	int _rectType;
	// TODO 0000008E field_8E		dw ?
	Mouse *_mouseCursor;
	Klayman *_klayman;
	Palette *_palette;
	Background *_background;
	bool _surfaceFlag;
	bool _messageListFlag;
	MessageList *_messageList2;
	int _messageListStatus;
	SmackerPlayer *_smackerPlayer;
	void (Entity::*_savedUpdateHandlerCb)();
	uint32 (Entity::*_savedMessageHandlerCb)(int messageNum, const MessageParam &param, Entity *sender);
	bool _smackerDone;
	// TODO 000000BD field_BD		db ?
	// TODO 000000BE field_BE		db ?
	// TODO 000000BF field_BF		db ?
	uint32 _smkFileHash;
	// TODO 000000C4 hitArray		dd ?
	bool _messageListFlag2;
	bool _prevVisible;
	int _messageValue;
	// TODO 000000CF field_CF		db ?
	uint32 handleMessage(int messageNum, const MessageParam &param, Entity *sender);
	void smackerUpdate();
	uint32 smackerHandleMessage(int messageNum, const MessageParam &param, Entity *sender);
	bool queryPositionSprite(int16 mouseX, int16 mouseY);
	bool queryPositionRectList(int16 mouseX, int16 mouseY);
	void setMessageList(uint32 id, bool messageListFlag = true, bool systemCallbackFlag = false);
	void setMessageList(MessageList *messageList, bool messageListFlag = true, bool systemCallbackFlag = false);
	bool setMessageList2(uint32 id, bool messageListFlag = true, bool systemCallbackFlag = false);
	bool setMessageList2(MessageList *messageList, bool messageListFlag = true, bool systemCallbackFlag = false);
	void runMessageList();
	void setRectList(uint32 id);
	void setRectList(RectList *rectList);
	void clearRectList();
	void loadHitRectList();
	void messageList402220();
	void loadDataResource(uint32 fileHash);
	uint16 convertMessageNum(uint32 messageNum);
};

} // End of namespace Neverhood

#endif /* NEVERHOOD_SCENE_H */
