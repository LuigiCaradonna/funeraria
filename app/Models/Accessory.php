<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;
use App\Models\AccessoryType;

class Accessory extends Model
{
    use HasFactory;
    
    /**
     * The attributes that are mass assignable.
     *
     * @var array<string, int>
     */
    protected $fillable = [
        'name',
        'accessory_type_id'
    ];

    /**
     * The relationships that should always be loaded.
     *
     * @var array
     */
    // protected $with = ['accessory_type'];

    /**
     * Get the accessory type that owns the accessory.
     */
    public function accessory_type()
    {
        return $this->belongsTo(AccessoryType::class, 'accessory_type_id');
    }
}
