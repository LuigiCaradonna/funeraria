<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;

class Tomb extends Model
{
    use HasFactory;
    
    /**
     * The attributes that are mass assignable.
     *
     * @var array<int, string, decimal, float, float, int, int, int, int, int, int, bool, int, int, int, int, int, int, int, string, date, date, date, date, date>
     */
    protected $fillable = [
        'client_id',
        'name',
        'price',
        'full_width',
        'full_height',
        'tomb_type_id', // 2+2 cassettone/base-tetto, 3 ...
        'finishing_id', // toro, toro-dritto, costa dritta
        'material_id',
        'font_id',
        'color_id',
        'sculpture_id',
        'polished',     // polished sculpture or not
        'vase_id',      // vase
        'lamp_id',      // lamp
        'flame_id',     // flame
        'photo_id',     // photo
        'frame_id',     // frame
        'cross_id',     // cross
        'decoration_id',// other decorations
        'note',
        'ordered_at',   // order date
        'proofed_at',   // date when the sketch has been sent
        'confirmed_at', // date of the order's confirmation
        'done_at',      // engraving date
        'delivered_at', // delivery date
    ];
    
    /**
     * Get the tomb type that owns the tomb.
     */
    public function tomb_type()
    {
        return $this->belongsTo(TombType::class, 'tomb_type_id');
    }
}
