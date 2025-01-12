// <auto-generated>
//  automatically generated by the FlatBuffers compiler, do not modify
// </auto-generated>

namespace Game.TheLastOne
{

using global::System;
using global::FlatBuffers;

public struct Game_HP_Set : IFlatbufferObject
{
  private Table __p;
  public ByteBuffer ByteBuffer { get { return __p.bb; } }
  public static Game_HP_Set GetRootAsGame_HP_Set(ByteBuffer _bb) { return GetRootAsGame_HP_Set(_bb, new Game_HP_Set()); }
  public static Game_HP_Set GetRootAsGame_HP_Set(ByteBuffer _bb, Game_HP_Set obj) { return (obj.__assign(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __p.bb_pos = _i; __p.bb = _bb; }
  public Game_HP_Set __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public int Id { get { int o = __p.__offset(4); return o != 0 ? __p.bb.GetInt(o + __p.bb_pos) : (int)0; } }
  public int Hp { get { int o = __p.__offset(6); return o != 0 ? __p.bb.GetInt(o + __p.bb_pos) : (int)0; } }
  public int Armour { get { int o = __p.__offset(8); return o != 0 ? __p.bb.GetInt(o + __p.bb_pos) : (int)0; } }
  public int Kind { get { int o = __p.__offset(10); return o != 0 ? __p.bb.GetInt(o + __p.bb_pos) : (int)0; } }

  public static Offset<Game_HP_Set> CreateGame_HP_Set(FlatBufferBuilder builder,
      int id = 0,
      int hp = 0,
      int armour = 0,
      int kind = 0) {
    builder.StartObject(4);
    Game_HP_Set.AddKind(builder, kind);
    Game_HP_Set.AddArmour(builder, armour);
    Game_HP_Set.AddHp(builder, hp);
    Game_HP_Set.AddId(builder, id);
    return Game_HP_Set.EndGame_HP_Set(builder);
  }

  public static void StartGame_HP_Set(FlatBufferBuilder builder) { builder.StartObject(4); }
  public static void AddId(FlatBufferBuilder builder, int id) { builder.AddInt(0, id, 0); }
  public static void AddHp(FlatBufferBuilder builder, int hp) { builder.AddInt(1, hp, 0); }
  public static void AddArmour(FlatBufferBuilder builder, int armour) { builder.AddInt(2, armour, 0); }
  public static void AddKind(FlatBufferBuilder builder, int kind) { builder.AddInt(3, kind, 0); }
  public static Offset<Game_HP_Set> EndGame_HP_Set(FlatBufferBuilder builder) {
    int o = builder.EndObject();
    return new Offset<Game_HP_Set>(o);
  }
};


}
