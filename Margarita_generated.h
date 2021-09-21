// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_MARGARITA_H_
#define FLATBUFFERS_GENERATED_MARGARITA_H_

#include "flatbuffers/flatbuffers.h"

struct NetPlayer;
struct NetPlayerBuilder;

struct NetEntity;
struct NetEntityBuilder;

struct ServerPacket;
struct ServerPacketBuilder;

struct ServerObject;
struct ServerObjectBuilder;

struct ServerObjectPacket;
struct ServerObjectPacketBuilder;

struct ServerNamePacket;
struct ServerNamePacketBuilder;

struct ClientPacket;
struct ClientPacketBuilder;

struct NetPlayer FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef NetPlayerBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_ID = 4,
    VT_FX = 6,
    VT_FY = 8,
    VT_TEXTURE = 10,
    VT_FRAME = 12,
    VT_HEALTH = 14,
    VT_ENERGY = 16,
    VT_ALIVE = 18,
    VT_READY = 20,
    VT_MONEY_GAINED = 22
  };
  int16_t id() const {
    return GetField<int16_t>(VT_ID, 0);
  }
  float fx() const {
    return GetField<float>(VT_FX, 0.0f);
  }
  float fy() const {
    return GetField<float>(VT_FY, 0.0f);
  }
  int8_t texture() const {
    return GetField<int8_t>(VT_TEXTURE, 0);
  }
  int8_t frame() const {
    return GetField<int8_t>(VT_FRAME, 0);
  }
  float health() const {
    return GetField<float>(VT_HEALTH, 0.0f);
  }
  float energy() const {
    return GetField<float>(VT_ENERGY, 0.0f);
  }
  bool alive() const {
    return GetField<uint8_t>(VT_ALIVE, 0) != 0;
  }
  bool ready() const {
    return GetField<uint8_t>(VT_READY, 0) != 0;
  }
  int32_t money_gained() const {
    return GetField<int32_t>(VT_MONEY_GAINED, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int16_t>(verifier, VT_ID) &&
           VerifyField<float>(verifier, VT_FX) &&
           VerifyField<float>(verifier, VT_FY) &&
           VerifyField<int8_t>(verifier, VT_TEXTURE) &&
           VerifyField<int8_t>(verifier, VT_FRAME) &&
           VerifyField<float>(verifier, VT_HEALTH) &&
           VerifyField<float>(verifier, VT_ENERGY) &&
           VerifyField<uint8_t>(verifier, VT_ALIVE) &&
           VerifyField<uint8_t>(verifier, VT_READY) &&
           VerifyField<int32_t>(verifier, VT_MONEY_GAINED) &&
           verifier.EndTable();
  }
};

struct NetPlayerBuilder {
  typedef NetPlayer Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_id(int16_t id) {
    fbb_.AddElement<int16_t>(NetPlayer::VT_ID, id, 0);
  }
  void add_fx(float fx) {
    fbb_.AddElement<float>(NetPlayer::VT_FX, fx, 0.0f);
  }
  void add_fy(float fy) {
    fbb_.AddElement<float>(NetPlayer::VT_FY, fy, 0.0f);
  }
  void add_texture(int8_t texture) {
    fbb_.AddElement<int8_t>(NetPlayer::VT_TEXTURE, texture, 0);
  }
  void add_frame(int8_t frame) {
    fbb_.AddElement<int8_t>(NetPlayer::VT_FRAME, frame, 0);
  }
  void add_health(float health) {
    fbb_.AddElement<float>(NetPlayer::VT_HEALTH, health, 0.0f);
  }
  void add_energy(float energy) {
    fbb_.AddElement<float>(NetPlayer::VT_ENERGY, energy, 0.0f);
  }
  void add_alive(bool alive) {
    fbb_.AddElement<uint8_t>(NetPlayer::VT_ALIVE, static_cast<uint8_t>(alive), 0);
  }
  void add_ready(bool ready) {
    fbb_.AddElement<uint8_t>(NetPlayer::VT_READY, static_cast<uint8_t>(ready), 0);
  }
  void add_money_gained(int32_t money_gained) {
    fbb_.AddElement<int32_t>(NetPlayer::VT_MONEY_GAINED, money_gained, 0);
  }
  explicit NetPlayerBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<NetPlayer> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<NetPlayer>(end);
    return o;
  }
};

inline flatbuffers::Offset<NetPlayer> CreateNetPlayer(
    flatbuffers::FlatBufferBuilder &_fbb,
    int16_t id = 0,
    float fx = 0.0f,
    float fy = 0.0f,
    int8_t texture = 0,
    int8_t frame = 0,
    float health = 0.0f,
    float energy = 0.0f,
    bool alive = false,
    bool ready = false,
    int32_t money_gained = 0) {
  NetPlayerBuilder builder_(_fbb);
  builder_.add_money_gained(money_gained);
  builder_.add_energy(energy);
  builder_.add_health(health);
  builder_.add_fy(fy);
  builder_.add_fx(fx);
  builder_.add_id(id);
  builder_.add_ready(ready);
  builder_.add_alive(alive);
  builder_.add_frame(frame);
  builder_.add_texture(texture);
  return builder_.Finish();
}

struct NetEntity FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef NetEntityBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_FX = 4,
    VT_FY = 6,
    VT_TEXTURE = 8,
    VT_ROTATION = 10,
    VT_FRAME = 12
  };
  float fx() const {
    return GetField<float>(VT_FX, 0.0f);
  }
  float fy() const {
    return GetField<float>(VT_FY, 0.0f);
  }
  int8_t texture() const {
    return GetField<int8_t>(VT_TEXTURE, 0);
  }
  float rotation() const {
    return GetField<float>(VT_ROTATION, 0.0f);
  }
  int8_t frame() const {
    return GetField<int8_t>(VT_FRAME, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<float>(verifier, VT_FX) &&
           VerifyField<float>(verifier, VT_FY) &&
           VerifyField<int8_t>(verifier, VT_TEXTURE) &&
           VerifyField<float>(verifier, VT_ROTATION) &&
           VerifyField<int8_t>(verifier, VT_FRAME) &&
           verifier.EndTable();
  }
};

struct NetEntityBuilder {
  typedef NetEntity Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_fx(float fx) {
    fbb_.AddElement<float>(NetEntity::VT_FX, fx, 0.0f);
  }
  void add_fy(float fy) {
    fbb_.AddElement<float>(NetEntity::VT_FY, fy, 0.0f);
  }
  void add_texture(int8_t texture) {
    fbb_.AddElement<int8_t>(NetEntity::VT_TEXTURE, texture, 0);
  }
  void add_rotation(float rotation) {
    fbb_.AddElement<float>(NetEntity::VT_ROTATION, rotation, 0.0f);
  }
  void add_frame(int8_t frame) {
    fbb_.AddElement<int8_t>(NetEntity::VT_FRAME, frame, 0);
  }
  explicit NetEntityBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<NetEntity> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<NetEntity>(end);
    return o;
  }
};

inline flatbuffers::Offset<NetEntity> CreateNetEntity(
    flatbuffers::FlatBufferBuilder &_fbb,
    float fx = 0.0f,
    float fy = 0.0f,
    int8_t texture = 0,
    float rotation = 0.0f,
    int8_t frame = 0) {
  NetEntityBuilder builder_(_fbb);
  builder_.add_rotation(rotation);
  builder_.add_fy(fy);
  builder_.add_fx(fx);
  builder_.add_frame(frame);
  builder_.add_texture(texture);
  return builder_.Finish();
}

struct ServerPacket FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef ServerPacketBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_TIME_LEFT = 4,
    VT_WAVE_NUMBER = 6,
    VT_WAVE_FINISHED = 8,
    VT_PLAYERS = 10,
    VT_ENTITIES = 12
  };
  float time_left() const {
    return GetField<float>(VT_TIME_LEFT, 0.0f);
  }
  int16_t wave_number() const {
    return GetField<int16_t>(VT_WAVE_NUMBER, 0);
  }
  bool wave_finished() const {
    return GetField<uint8_t>(VT_WAVE_FINISHED, 0) != 0;
  }
  const flatbuffers::Vector<flatbuffers::Offset<NetPlayer>> *players() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<NetPlayer>> *>(VT_PLAYERS);
  }
  const flatbuffers::Vector<flatbuffers::Offset<NetEntity>> *entities() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<NetEntity>> *>(VT_ENTITIES);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<float>(verifier, VT_TIME_LEFT) &&
           VerifyField<int16_t>(verifier, VT_WAVE_NUMBER) &&
           VerifyField<uint8_t>(verifier, VT_WAVE_FINISHED) &&
           VerifyOffset(verifier, VT_PLAYERS) &&
           verifier.VerifyVector(players()) &&
           verifier.VerifyVectorOfTables(players()) &&
           VerifyOffset(verifier, VT_ENTITIES) &&
           verifier.VerifyVector(entities()) &&
           verifier.VerifyVectorOfTables(entities()) &&
           verifier.EndTable();
  }
};

struct ServerPacketBuilder {
  typedef ServerPacket Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_time_left(float time_left) {
    fbb_.AddElement<float>(ServerPacket::VT_TIME_LEFT, time_left, 0.0f);
  }
  void add_wave_number(int16_t wave_number) {
    fbb_.AddElement<int16_t>(ServerPacket::VT_WAVE_NUMBER, wave_number, 0);
  }
  void add_wave_finished(bool wave_finished) {
    fbb_.AddElement<uint8_t>(ServerPacket::VT_WAVE_FINISHED, static_cast<uint8_t>(wave_finished), 0);
  }
  void add_players(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<NetPlayer>>> players) {
    fbb_.AddOffset(ServerPacket::VT_PLAYERS, players);
  }
  void add_entities(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<NetEntity>>> entities) {
    fbb_.AddOffset(ServerPacket::VT_ENTITIES, entities);
  }
  explicit ServerPacketBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<ServerPacket> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<ServerPacket>(end);
    return o;
  }
};

inline flatbuffers::Offset<ServerPacket> CreateServerPacket(
    flatbuffers::FlatBufferBuilder &_fbb,
    float time_left = 0.0f,
    int16_t wave_number = 0,
    bool wave_finished = false,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<NetPlayer>>> players = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<NetEntity>>> entities = 0) {
  ServerPacketBuilder builder_(_fbb);
  builder_.add_entities(entities);
  builder_.add_players(players);
  builder_.add_time_left(time_left);
  builder_.add_wave_number(wave_number);
  builder_.add_wave_finished(wave_finished);
  return builder_.Finish();
}

inline flatbuffers::Offset<ServerPacket> CreateServerPacketDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    float time_left = 0.0f,
    int16_t wave_number = 0,
    bool wave_finished = false,
    const std::vector<flatbuffers::Offset<NetPlayer>> *players = nullptr,
    const std::vector<flatbuffers::Offset<NetEntity>> *entities = nullptr) {
  auto players__ = players ? _fbb.CreateVector<flatbuffers::Offset<NetPlayer>>(*players) : 0;
  auto entities__ = entities ? _fbb.CreateVector<flatbuffers::Offset<NetEntity>>(*entities) : 0;
  return CreateServerPacket(
      _fbb,
      time_left,
      wave_number,
      wave_finished,
      players__,
      entities__);
}

struct ServerObject FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef ServerObjectBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_TYPE = 4,
    VT_DATA = 6,
    VT_FX = 8,
    VT_FY = 10
  };
  int8_t type() const {
    return GetField<int8_t>(VT_TYPE, 0);
  }
  int16_t data() const {
    return GetField<int16_t>(VT_DATA, 0);
  }
  float fx() const {
    return GetField<float>(VT_FX, 0.0f);
  }
  float fy() const {
    return GetField<float>(VT_FY, 0.0f);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int8_t>(verifier, VT_TYPE) &&
           VerifyField<int16_t>(verifier, VT_DATA) &&
           VerifyField<float>(verifier, VT_FX) &&
           VerifyField<float>(verifier, VT_FY) &&
           verifier.EndTable();
  }
};

struct ServerObjectBuilder {
  typedef ServerObject Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_type(int8_t type) {
    fbb_.AddElement<int8_t>(ServerObject::VT_TYPE, type, 0);
  }
  void add_data(int16_t data) {
    fbb_.AddElement<int16_t>(ServerObject::VT_DATA, data, 0);
  }
  void add_fx(float fx) {
    fbb_.AddElement<float>(ServerObject::VT_FX, fx, 0.0f);
  }
  void add_fy(float fy) {
    fbb_.AddElement<float>(ServerObject::VT_FY, fy, 0.0f);
  }
  explicit ServerObjectBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<ServerObject> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<ServerObject>(end);
    return o;
  }
};

inline flatbuffers::Offset<ServerObject> CreateServerObject(
    flatbuffers::FlatBufferBuilder &_fbb,
    int8_t type = 0,
    int16_t data = 0,
    float fx = 0.0f,
    float fy = 0.0f) {
  ServerObjectBuilder builder_(_fbb);
  builder_.add_fy(fy);
  builder_.add_fx(fx);
  builder_.add_data(data);
  builder_.add_type(type);
  return builder_.Finish();
}

struct ServerObjectPacket FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef ServerObjectPacketBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_OBJECTS = 4
  };
  const flatbuffers::Vector<flatbuffers::Offset<ServerObject>> *objects() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<ServerObject>> *>(VT_OBJECTS);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_OBJECTS) &&
           verifier.VerifyVector(objects()) &&
           verifier.VerifyVectorOfTables(objects()) &&
           verifier.EndTable();
  }
};

struct ServerObjectPacketBuilder {
  typedef ServerObjectPacket Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_objects(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<ServerObject>>> objects) {
    fbb_.AddOffset(ServerObjectPacket::VT_OBJECTS, objects);
  }
  explicit ServerObjectPacketBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<ServerObjectPacket> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<ServerObjectPacket>(end);
    return o;
  }
};

inline flatbuffers::Offset<ServerObjectPacket> CreateServerObjectPacket(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<ServerObject>>> objects = 0) {
  ServerObjectPacketBuilder builder_(_fbb);
  builder_.add_objects(objects);
  return builder_.Finish();
}

inline flatbuffers::Offset<ServerObjectPacket> CreateServerObjectPacketDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<flatbuffers::Offset<ServerObject>> *objects = nullptr) {
  auto objects__ = objects ? _fbb.CreateVector<flatbuffers::Offset<ServerObject>>(*objects) : 0;
  return CreateServerObjectPacket(
      _fbb,
      objects__);
}

struct ServerNamePacket FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef ServerNamePacketBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_NAMES = 4
  };
  const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>> *names() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>> *>(VT_NAMES);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_NAMES) &&
           verifier.VerifyVector(names()) &&
           verifier.VerifyVectorOfStrings(names()) &&
           verifier.EndTable();
  }
};

struct ServerNamePacketBuilder {
  typedef ServerNamePacket Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_names(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>> names) {
    fbb_.AddOffset(ServerNamePacket::VT_NAMES, names);
  }
  explicit ServerNamePacketBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<ServerNamePacket> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<ServerNamePacket>(end);
    return o;
  }
};

inline flatbuffers::Offset<ServerNamePacket> CreateServerNamePacket(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>> names = 0) {
  ServerNamePacketBuilder builder_(_fbb);
  builder_.add_names(names);
  return builder_.Finish();
}

inline flatbuffers::Offset<ServerNamePacket> CreateServerNamePacketDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<flatbuffers::Offset<flatbuffers::String>> *names = nullptr) {
  auto names__ = names ? _fbb.CreateVector<flatbuffers::Offset<flatbuffers::String>>(*names) : 0;
  return CreateServerNamePacket(
      _fbb,
      names__);
}

struct ClientPacket FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef ClientPacketBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_WEAPON_FIRED = 4,
    VT_WEAPON_FIRED_ANGLE = 6,
    VT_WEAPON_FIRED_INTENSITY = 8,
    VT_USING_POWERUP = 10,
    VT_READY = 12,
    VT_FX = 14,
    VT_FY = 16,
    VT_MAX_HEALTH = 18,
    VT_MAX_ENERGY = 20
  };
  int8_t weapon_fired() const {
    return GetField<int8_t>(VT_WEAPON_FIRED, 0);
  }
  float weapon_fired_angle() const {
    return GetField<float>(VT_WEAPON_FIRED_ANGLE, 0.0f);
  }
  int8_t weapon_fired_intensity() const {
    return GetField<int8_t>(VT_WEAPON_FIRED_INTENSITY, 0);
  }
  bool using_powerup() const {
    return GetField<uint8_t>(VT_USING_POWERUP, 0) != 0;
  }
  bool ready() const {
    return GetField<uint8_t>(VT_READY, 0) != 0;
  }
  float fx() const {
    return GetField<float>(VT_FX, 0.0f);
  }
  float fy() const {
    return GetField<float>(VT_FY, 0.0f);
  }
  float max_health() const {
    return GetField<float>(VT_MAX_HEALTH, 0.0f);
  }
  float max_energy() const {
    return GetField<float>(VT_MAX_ENERGY, 0.0f);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int8_t>(verifier, VT_WEAPON_FIRED) &&
           VerifyField<float>(verifier, VT_WEAPON_FIRED_ANGLE) &&
           VerifyField<int8_t>(verifier, VT_WEAPON_FIRED_INTENSITY) &&
           VerifyField<uint8_t>(verifier, VT_USING_POWERUP) &&
           VerifyField<uint8_t>(verifier, VT_READY) &&
           VerifyField<float>(verifier, VT_FX) &&
           VerifyField<float>(verifier, VT_FY) &&
           VerifyField<float>(verifier, VT_MAX_HEALTH) &&
           VerifyField<float>(verifier, VT_MAX_ENERGY) &&
           verifier.EndTable();
  }
};

struct ClientPacketBuilder {
  typedef ClientPacket Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_weapon_fired(int8_t weapon_fired) {
    fbb_.AddElement<int8_t>(ClientPacket::VT_WEAPON_FIRED, weapon_fired, 0);
  }
  void add_weapon_fired_angle(float weapon_fired_angle) {
    fbb_.AddElement<float>(ClientPacket::VT_WEAPON_FIRED_ANGLE, weapon_fired_angle, 0.0f);
  }
  void add_weapon_fired_intensity(int8_t weapon_fired_intensity) {
    fbb_.AddElement<int8_t>(ClientPacket::VT_WEAPON_FIRED_INTENSITY, weapon_fired_intensity, 0);
  }
  void add_using_powerup(bool using_powerup) {
    fbb_.AddElement<uint8_t>(ClientPacket::VT_USING_POWERUP, static_cast<uint8_t>(using_powerup), 0);
  }
  void add_ready(bool ready) {
    fbb_.AddElement<uint8_t>(ClientPacket::VT_READY, static_cast<uint8_t>(ready), 0);
  }
  void add_fx(float fx) {
    fbb_.AddElement<float>(ClientPacket::VT_FX, fx, 0.0f);
  }
  void add_fy(float fy) {
    fbb_.AddElement<float>(ClientPacket::VT_FY, fy, 0.0f);
  }
  void add_max_health(float max_health) {
    fbb_.AddElement<float>(ClientPacket::VT_MAX_HEALTH, max_health, 0.0f);
  }
  void add_max_energy(float max_energy) {
    fbb_.AddElement<float>(ClientPacket::VT_MAX_ENERGY, max_energy, 0.0f);
  }
  explicit ClientPacketBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<ClientPacket> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<ClientPacket>(end);
    return o;
  }
};

inline flatbuffers::Offset<ClientPacket> CreateClientPacket(
    flatbuffers::FlatBufferBuilder &_fbb,
    int8_t weapon_fired = 0,
    float weapon_fired_angle = 0.0f,
    int8_t weapon_fired_intensity = 0,
    bool using_powerup = false,
    bool ready = false,
    float fx = 0.0f,
    float fy = 0.0f,
    float max_health = 0.0f,
    float max_energy = 0.0f) {
  ClientPacketBuilder builder_(_fbb);
  builder_.add_max_energy(max_energy);
  builder_.add_max_health(max_health);
  builder_.add_fy(fy);
  builder_.add_fx(fx);
  builder_.add_weapon_fired_angle(weapon_fired_angle);
  builder_.add_ready(ready);
  builder_.add_using_powerup(using_powerup);
  builder_.add_weapon_fired_intensity(weapon_fired_intensity);
  builder_.add_weapon_fired(weapon_fired);
  return builder_.Finish();
}

#endif  // FLATBUFFERS_GENERATED_MARGARITA_H_
