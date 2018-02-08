// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_FLATBUFFERS_GAME_THELASTONE_H_
#define FLATBUFFERS_GENERATED_FLATBUFFERS_GAME_THELASTONE_H_

#include "flatbuffers/flatbuffers.h"

namespace Game {
	namespace TheLastOne {

		struct Vec3;

		struct Client_info;

		struct Client_id;

		MANUALLY_ALIGNED_STRUCT(4) Vec3 FLATBUFFERS_FINAL_CLASS {
		private:
			float x_;
			float y_;
			float z_;

		public:
			Vec3() {
				memset(this, 0, sizeof(Vec3));
			}
			Vec3(const Vec3 &_o) {
				memcpy(this, &_o, sizeof(Vec3));
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

		struct Client_info FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
			enum {
				VT_ID = 4,
				VT_HP = 6,
				VT_NAME = 8,
				VT_XYZ = 10,
				VT_ROTATION = 12
			};
			int32_t id() const {
				return GetField<int32_t>(VT_ID, 0);
			}
			int32_t hp() const {
				return GetField<int32_t>(VT_HP, 0);
			}
			const flatbuffers::String *name() const {
				return GetPointer<const flatbuffers::String *>(VT_NAME);
			}
			const Vec3 *xyz() const {
				return GetStruct<const Vec3 *>(VT_XYZ);
			}
			const Vec3 *rotation() const {
				return GetStruct<const Vec3 *>(VT_ROTATION);
			}
			bool Verify(flatbuffers::Verifier &verifier) const {
				return VerifyTableStart(verifier) &&
					VerifyField<int32_t>(verifier, VT_ID) &&
					VerifyField<int32_t>(verifier, VT_HP) &&
					VerifyOffset(verifier, VT_NAME) &&
					verifier.Verify(name()) &&
					VerifyField<Vec3>(verifier, VT_XYZ) &&
					VerifyField<Vec3>(verifier, VT_ROTATION) &&
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
			void add_name(flatbuffers::Offset<flatbuffers::String> name) {
				fbb_.AddOffset(Client_info::VT_NAME, name);
			}
			void add_xyz(const Vec3 *xyz) {
				fbb_.AddStruct(Client_info::VT_XYZ, xyz);
			}
			void add_rotation(const Vec3 *rotation) {
				fbb_.AddStruct(Client_info::VT_ROTATION, rotation);
			}
			Client_infoBuilder(flatbuffers::FlatBufferBuilder &_fbb)
				: fbb_(_fbb) {
				start_ = fbb_.StartTable();
			}
			Client_infoBuilder &operator=(const Client_infoBuilder &);
			flatbuffers::Offset<Client_info> Finish() {
				const auto end = fbb_.EndTable(start_, 5);
				auto o = flatbuffers::Offset<Client_info>(end);
				return o;
			}
		};

		inline flatbuffers::Offset<Client_info> CreateClient_info(
			flatbuffers::FlatBufferBuilder &_fbb,
			int32_t id = 0,
			int32_t hp = 0,
			flatbuffers::Offset<flatbuffers::String> name = 0,
			const Vec3 *xyz = 0,
			const Vec3 *rotation = 0) {
			Client_infoBuilder builder_(_fbb);
			builder_.add_rotation(rotation);
			builder_.add_xyz(xyz);
			builder_.add_name(name);
			builder_.add_hp(hp);
			builder_.add_id(id);
			return builder_.Finish();
		}

		inline flatbuffers::Offset<Client_info> CreateClient_infoDirect(
			flatbuffers::FlatBufferBuilder &_fbb,
			int32_t id = 0,
			int32_t hp = 0,
			const char *name = nullptr,
			const Vec3 *xyz = 0,
			const Vec3 *rotation = 0) {
			return Game::TheLastOne::CreateClient_info(
				_fbb,
				id,
				hp,
				name ? _fbb.CreateString(name) : 0,
				xyz,
				rotation);
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
			Client_idBuilder(flatbuffers::FlatBufferBuilder &_fbb)
				: fbb_(_fbb) {
				start_ = fbb_.StartTable();
			}
			Client_idBuilder &operator=(const Client_idBuilder &);
			flatbuffers::Offset<Client_id> Finish() {
				const auto end = fbb_.EndTable(start_, 1);
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

		/* 왜인지 모르겠지만 Get이 없어서 샘플에서 가져왔다..! */
		inline const Game::TheLastOne::Client_info *GetClientView(const void *buf) {
			return flatbuffers::GetRoot<Game::TheLastOne::Client_info>(buf);
		}

	}  // namespace TheLastOne
}  // namespace Game

#endif  // FLATBUFFERS_GENERATED_FLATBUFFERS_GAME_THELASTONE_H_
