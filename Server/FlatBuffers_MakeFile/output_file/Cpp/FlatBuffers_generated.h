// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_FLATBUFFERS_GAME_THELASTONE_H_
#define FLATBUFFERS_GENERATED_FLATBUFFERS_GAME_THELASTONE_H_

#include "flatbuffers/flatbuffers.h"

namespace Game {
namespace TheLastOne {

struct Vec3;

struct All_information;

struct Client_info;

struct Game_Items;

struct Gameitem;

struct Game_Timer;

struct Client_id;

struct Client_Shot_info;

MANUALLY_ALIGNED_STRUCT(4) Vec3 FLATBUFFERS_FINAL_CLASS {
 private:
  float x_;
  float y_;
  float z_;

 public:
  Vec3() {
    memset(this, 0, sizeof(Vec3));
  }
  Vec3(float _x, float _y, float _z)
      : x_(flatbuffers::EndianScalar(_x)),
        y_(flatbuffers::EndianScalar(_y)),
        z_(flatbuffers::EndianScalar(_z)) {
  }
  float x() const {
    return flatbuffers::EndianScalar(x_);
  }
  float y() const {
    return flatbuffers::EndianScalar(y_);
  }
  float z() const {
    return flatbuffers::EndianScalar(z_);
  }
};
STRUCT_END(Vec3, 12);

struct All_information FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_DATA = 4
  };
  const flatbuffers::Vector<flatbuffers::Offset<Client_info>> *data() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Client_info>> *>(VT_DATA);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_DATA) &&
           verifier.Verify(data()) &&
           verifier.VerifyVectorOfTables(data()) &&
           verifier.EndTable();
  }
};

struct All_informationBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_data(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Client_info>>> data) {
    fbb_.AddOffset(All_information::VT_DATA, data);
  }
  explicit All_informationBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  All_informationBuilder &operator=(const All_informationBuilder &);
  flatbuffers::Offset<All_information> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<All_information>(end);
    return o;
  }
};

inline flatbuffers::Offset<All_information> CreateAll_information(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Client_info>>> data = 0) {
  All_informationBuilder builder_(_fbb);
  builder_.add_data(data);
  return builder_.Finish();
}

inline flatbuffers::Offset<All_information> CreateAll_informationDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<flatbuffers::Offset<Client_info>> *data = nullptr) {
  return Game::TheLastOne::CreateAll_information(
      _fbb,
      data ? _fbb.CreateVector<flatbuffers::Offset<Client_info>>(*data) : 0);
}

struct Client_info FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_ID = 4,
    VT_HP = 6,
    VT_ANIMATOR = 8,
    VT_NAME = 10,
    VT_POSITION = 12,
    VT_ROTATION = 14,
    VT_NOWWEAPON = 16
  };
  int32_t id() const {
    return GetField<int32_t>(VT_ID, 0);
  }
  int32_t hp() const {
    return GetField<int32_t>(VT_HP, 0);
  }
  int32_t animator() const {
    return GetField<int32_t>(VT_ANIMATOR, 0);
  }
  const flatbuffers::String *name() const {
    return GetPointer<const flatbuffers::String *>(VT_NAME);
  }
  const Vec3 *position() const {
    return GetStruct<const Vec3 *>(VT_POSITION);
  }
  const Vec3 *rotation() const {
    return GetStruct<const Vec3 *>(VT_ROTATION);
  }
  int32_t nowWeapon() const {
    return GetField<int32_t>(VT_NOWWEAPON, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int32_t>(verifier, VT_ID) &&
           VerifyField<int32_t>(verifier, VT_HP) &&
           VerifyField<int32_t>(verifier, VT_ANIMATOR) &&
           VerifyOffset(verifier, VT_NAME) &&
           verifier.Verify(name()) &&
           VerifyField<Vec3>(verifier, VT_POSITION) &&
           VerifyField<Vec3>(verifier, VT_ROTATION) &&
           VerifyField<int32_t>(verifier, VT_NOWWEAPON) &&
           verifier.EndTable();
  }
};

struct Client_infoBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_id(int32_t id) {
    fbb_.AddElement<int32_t>(Client_info::VT_ID, id, 0);
  }
  void add_hp(int32_t hp) {
    fbb_.AddElement<int32_t>(Client_info::VT_HP, hp, 0);
  }
  void add_animator(int32_t animator) {
    fbb_.AddElement<int32_t>(Client_info::VT_ANIMATOR, animator, 0);
  }
  void add_name(flatbuffers::Offset<flatbuffers::String> name) {
    fbb_.AddOffset(Client_info::VT_NAME, name);
  }
  void add_position(const Vec3 *position) {
    fbb_.AddStruct(Client_info::VT_POSITION, position);
  }
  void add_rotation(const Vec3 *rotation) {
    fbb_.AddStruct(Client_info::VT_ROTATION, rotation);
  }
  void add_nowWeapon(int32_t nowWeapon) {
    fbb_.AddElement<int32_t>(Client_info::VT_NOWWEAPON, nowWeapon, 0);
  }
  explicit Client_infoBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  Client_infoBuilder &operator=(const Client_infoBuilder &);
  flatbuffers::Offset<Client_info> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Client_info>(end);
    return o;
  }
};

inline flatbuffers::Offset<Client_info> CreateClient_info(
    flatbuffers::FlatBufferBuilder &_fbb,
    int32_t id = 0,
    int32_t hp = 0,
    int32_t animator = 0,
    flatbuffers::Offset<flatbuffers::String> name = 0,
    const Vec3 *position = 0,
    const Vec3 *rotation = 0,
    int32_t nowWeapon = 0) {
  Client_infoBuilder builder_(_fbb);
  builder_.add_nowWeapon(nowWeapon);
  builder_.add_rotation(rotation);
  builder_.add_position(position);
  builder_.add_name(name);
  builder_.add_animator(animator);
  builder_.add_hp(hp);
  builder_.add_id(id);
  return builder_.Finish();
}

inline flatbuffers::Offset<Client_info> CreateClient_infoDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    int32_t id = 0,
    int32_t hp = 0,
    int32_t animator = 0,
    const char *name = nullptr,
    const Vec3 *position = 0,
    const Vec3 *rotation = 0,
    int32_t nowWeapon = 0) {
  return Game::TheLastOne::CreateClient_info(
      _fbb,
      id,
      hp,
      animator,
      name ? _fbb.CreateString(name) : 0,
      position,
      rotation,
      nowWeapon);
}

struct Game_Items FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_DATA = 4
  };
  const flatbuffers::Vector<flatbuffers::Offset<Gameitem>> *data() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Gameitem>> *>(VT_DATA);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_DATA) &&
           verifier.Verify(data()) &&
           verifier.VerifyVectorOfTables(data()) &&
           verifier.EndTable();
  }
};

struct Game_ItemsBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_data(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Gameitem>>> data) {
    fbb_.AddOffset(Game_Items::VT_DATA, data);
  }
  explicit Game_ItemsBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  Game_ItemsBuilder &operator=(const Game_ItemsBuilder &);
  flatbuffers::Offset<Game_Items> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Game_Items>(end);
    return o;
  }
};

inline flatbuffers::Offset<Game_Items> CreateGame_Items(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Gameitem>>> data = 0) {
  Game_ItemsBuilder builder_(_fbb);
  builder_.add_data(data);
  return builder_.Finish();
}

inline flatbuffers::Offset<Game_Items> CreateGame_ItemsDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<flatbuffers::Offset<Gameitem>> *data = nullptr) {
  return Game::TheLastOne::CreateGame_Items(
      _fbb,
      data ? _fbb.CreateVector<flatbuffers::Offset<Gameitem>>(*data) : 0);
}

struct Gameitem FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_ID = 4,
    VT_NAME = 6,
    VT_X = 8,
    VT_Z = 10,
    VT_EAT = 12
  };
  int32_t id() const {
    return GetField<int32_t>(VT_ID, 0);
  }
  const flatbuffers::String *name() const {
    return GetPointer<const flatbuffers::String *>(VT_NAME);
  }
  float x() const {
    return GetField<float>(VT_X, 0.0f);
  }
  float z() const {
    return GetField<float>(VT_Z, 0.0f);
  }
  bool eat() const {
    return GetField<uint8_t>(VT_EAT, 0) != 0;
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int32_t>(verifier, VT_ID) &&
           VerifyOffset(verifier, VT_NAME) &&
           verifier.Verify(name()) &&
           VerifyField<float>(verifier, VT_X) &&
           VerifyField<float>(verifier, VT_Z) &&
           VerifyField<uint8_t>(verifier, VT_EAT) &&
           verifier.EndTable();
  }
};

struct GameitemBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_id(int32_t id) {
    fbb_.AddElement<int32_t>(Gameitem::VT_ID, id, 0);
  }
  void add_name(flatbuffers::Offset<flatbuffers::String> name) {
    fbb_.AddOffset(Gameitem::VT_NAME, name);
  }
  void add_x(float x) {
    fbb_.AddElement<float>(Gameitem::VT_X, x, 0.0f);
  }
  void add_z(float z) {
    fbb_.AddElement<float>(Gameitem::VT_Z, z, 0.0f);
  }
  void add_eat(bool eat) {
    fbb_.AddElement<uint8_t>(Gameitem::VT_EAT, static_cast<uint8_t>(eat), 0);
  }
  explicit GameitemBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  GameitemBuilder &operator=(const GameitemBuilder &);
  flatbuffers::Offset<Gameitem> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Gameitem>(end);
    return o;
  }
};

inline flatbuffers::Offset<Gameitem> CreateGameitem(
    flatbuffers::FlatBufferBuilder &_fbb,
    int32_t id = 0,
    flatbuffers::Offset<flatbuffers::String> name = 0,
    float x = 0.0f,
    float z = 0.0f,
    bool eat = false) {
  GameitemBuilder builder_(_fbb);
  builder_.add_z(z);
  builder_.add_x(x);
  builder_.add_name(name);
  builder_.add_id(id);
  builder_.add_eat(eat);
  return builder_.Finish();
}

inline flatbuffers::Offset<Gameitem> CreateGameitemDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    int32_t id = 0,
    const char *name = nullptr,
    float x = 0.0f,
    float z = 0.0f,
    bool eat = false) {
  return Game::TheLastOne::CreateGameitem(
      _fbb,
      id,
      name ? _fbb.CreateString(name) : 0,
      x,
      z,
      eat);
}

struct Game_Timer FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_KIND = 4,
    VT_TIME = 6
  };
  int32_t kind() const {
    return GetField<int32_t>(VT_KIND, 0);
  }
  int32_t time() const {
    return GetField<int32_t>(VT_TIME, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int32_t>(verifier, VT_KIND) &&
           VerifyField<int32_t>(verifier, VT_TIME) &&
           verifier.EndTable();
  }
};

struct Game_TimerBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_kind(int32_t kind) {
    fbb_.AddElement<int32_t>(Game_Timer::VT_KIND, kind, 0);
  }
  void add_time(int32_t time) {
    fbb_.AddElement<int32_t>(Game_Timer::VT_TIME, time, 0);
  }
  explicit Game_TimerBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  Game_TimerBuilder &operator=(const Game_TimerBuilder &);
  flatbuffers::Offset<Game_Timer> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Game_Timer>(end);
    return o;
  }
};

inline flatbuffers::Offset<Game_Timer> CreateGame_Timer(
    flatbuffers::FlatBufferBuilder &_fbb,
    int32_t kind = 0,
    int32_t time = 0) {
  Game_TimerBuilder builder_(_fbb);
  builder_.add_time(time);
  builder_.add_kind(kind);
  return builder_.Finish();
}

struct Client_id FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_ID = 4
  };
  int32_t id() const {
    return GetField<int32_t>(VT_ID, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int32_t>(verifier, VT_ID) &&
           verifier.EndTable();
  }
};

struct Client_idBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_id(int32_t id) {
    fbb_.AddElement<int32_t>(Client_id::VT_ID, id, 0);
  }
  explicit Client_idBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  Client_idBuilder &operator=(const Client_idBuilder &);
  flatbuffers::Offset<Client_id> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Client_id>(end);
    return o;
  }
};

inline flatbuffers::Offset<Client_id> CreateClient_id(
    flatbuffers::FlatBufferBuilder &_fbb,
    int32_t id = 0) {
  Client_idBuilder builder_(_fbb);
  builder_.add_id(id);
  return builder_.Finish();
}

struct Client_Shot_info FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_ID = 4
  };
  int32_t id() const {
    return GetField<int32_t>(VT_ID, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int32_t>(verifier, VT_ID) &&
           verifier.EndTable();
  }
};

struct Client_Shot_infoBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_id(int32_t id) {
    fbb_.AddElement<int32_t>(Client_Shot_info::VT_ID, id, 0);
  }
  explicit Client_Shot_infoBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  Client_Shot_infoBuilder &operator=(const Client_Shot_infoBuilder &);
  flatbuffers::Offset<Client_Shot_info> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Client_Shot_info>(end);
    return o;
  }
};

inline flatbuffers::Offset<Client_Shot_info> CreateClient_Shot_info(
    flatbuffers::FlatBufferBuilder &_fbb,
    int32_t id = 0) {
  Client_Shot_infoBuilder builder_(_fbb);
  builder_.add_id(id);
  return builder_.Finish();
}

}  // namespace TheLastOne
}  // namespace Game

#endif  // FLATBUFFERS_GENERATED_FLATBUFFERS_GAME_THELASTONE_H_
