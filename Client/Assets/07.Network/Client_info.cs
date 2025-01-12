// <auto-generated>
//  automatically generated by the FlatBuffers compiler, do not modify
// </auto-generated>

namespace Game.TheLastOne
{

using global::System;
using global::FlatBuffers;

public struct Client_info : IFlatbufferObject
{
  private Table __p;
  public ByteBuffer ByteBuffer { get { return __p.bb; } }
  public static Client_info GetRootAsClient_info(ByteBuffer _bb) { return GetRootAsClient_info(_bb, new Client_info()); }
  public static Client_info GetRootAsClient_info(ByteBuffer _bb, Client_info obj) { return (obj.__assign(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __p.bb_pos = _i; __p.bb = _bb; }
  public Client_info __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public int Id { get { int o = __p.__offset(4); return o != 0 ? __p.bb.GetInt(o + __p.bb_pos) : (int)0; } }
  public int Hp { get { int o = __p.__offset(6); return o != 0 ? __p.bb.GetInt(o + __p.bb_pos) : (int)0; } }
  public int Armour { get { int o = __p.__offset(8); return o != 0 ? __p.bb.GetInt(o + __p.bb_pos) : (int)0; } }
  public int Animator { get { int o = __p.__offset(10); return o != 0 ? __p.bb.GetInt(o + __p.bb_pos) : (int)0; } }
  public float Horizontal { get { int o = __p.__offset(12); return o != 0 ? __p.bb.GetFloat(o + __p.bb_pos) : (float)0.0f; } }
  public float Vertical { get { int o = __p.__offset(14); return o != 0 ? __p.bb.GetFloat(o + __p.bb_pos) : (float)0.0f; } }
  public int InCar { get { int o = __p.__offset(16); return o != 0 ? __p.bb.GetInt(o + __p.bb_pos) : (int)0; } }
  public string Name { get { int o = __p.__offset(18); return o != 0 ? __p.__string(o + __p.bb_pos) : null; } }
  public ArraySegment<byte>? GetNameBytes() { return __p.__vector_as_arraysegment(18); }
  public Vec3? Position { get { int o = __p.__offset(20); return o != 0 ? (Vec3?)(new Vec3()).__assign(o + __p.bb_pos, __p.bb) : null; } }
  public Vec3? Rotation { get { int o = __p.__offset(22); return o != 0 ? (Vec3?)(new Vec3()).__assign(o + __p.bb_pos, __p.bb) : null; } }
  public Vec3? Carrotation { get { int o = __p.__offset(24); return o != 0 ? (Vec3?)(new Vec3()).__assign(o + __p.bb_pos, __p.bb) : null; } }
  public float Carkmh { get { int o = __p.__offset(26); return o != 0 ? __p.bb.GetFloat(o + __p.bb_pos) : (float)0.0f; } }
  public bool DangerLineIn { get { int o = __p.__offset(28); return o != 0 ? 0!=__p.bb.Get(o + __p.bb_pos) : (bool)false; } }
  public int NowWeapon { get { int o = __p.__offset(30); return o != 0 ? __p.bb.GetInt(o + __p.bb_pos) : (int)0; } }
  public bool PlayerDie { get { int o = __p.__offset(32); return o != 0 ? 0!=__p.bb.Get(o + __p.bb_pos) : (bool)false; } }
  public int CostumNum { get { int o = __p.__offset(34); return o != 0 ? __p.bb.GetInt(o + __p.bb_pos) : (int)0; } }

  public static void StartClient_info(FlatBufferBuilder builder) { builder.StartObject(16); }
  public static void AddId(FlatBufferBuilder builder, int id) { builder.AddInt(0, id, 0); }
  public static void AddHp(FlatBufferBuilder builder, int hp) { builder.AddInt(1, hp, 0); }
  public static void AddArmour(FlatBufferBuilder builder, int armour) { builder.AddInt(2, armour, 0); }
  public static void AddAnimator(FlatBufferBuilder builder, int animator) { builder.AddInt(3, animator, 0); }
  public static void AddHorizontal(FlatBufferBuilder builder, float Horizontal) { builder.AddFloat(4, Horizontal, 0.0f); }
  public static void AddVertical(FlatBufferBuilder builder, float Vertical) { builder.AddFloat(5, Vertical, 0.0f); }
  public static void AddInCar(FlatBufferBuilder builder, int inCar) { builder.AddInt(6, inCar, 0); }
  public static void AddName(FlatBufferBuilder builder, StringOffset nameOffset) { builder.AddOffset(7, nameOffset.Value, 0); }
  public static void AddPosition(FlatBufferBuilder builder, Offset<Vec3> positionOffset) { builder.AddStruct(8, positionOffset.Value, 0); }
  public static void AddRotation(FlatBufferBuilder builder, Offset<Vec3> rotationOffset) { builder.AddStruct(9, rotationOffset.Value, 0); }
  public static void AddCarrotation(FlatBufferBuilder builder, Offset<Vec3> carrotationOffset) { builder.AddStruct(10, carrotationOffset.Value, 0); }
  public static void AddCarkmh(FlatBufferBuilder builder, float carkmh) { builder.AddFloat(11, carkmh, 0.0f); }
  public static void AddDangerLineIn(FlatBufferBuilder builder, bool dangerLineIn) { builder.AddBool(12, dangerLineIn, false); }
  public static void AddNowWeapon(FlatBufferBuilder builder, int nowWeapon) { builder.AddInt(13, nowWeapon, 0); }
  public static void AddPlayerDie(FlatBufferBuilder builder, bool playerDie) { builder.AddBool(14, playerDie, false); }
  public static void AddCostumNum(FlatBufferBuilder builder, int costumNum) { builder.AddInt(15, costumNum, 0); }
  public static Offset<Client_info> EndClient_info(FlatBufferBuilder builder) {
    int o = builder.EndObject();
    return new Offset<Client_info>(o);
  }
};


}
