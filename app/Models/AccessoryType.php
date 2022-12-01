<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;
use App\Models\Accessory;

class AccessoryType extends Model
{
    use HasFactory;
    
    /**
     * The attributes that are mass assignable.
     *
     * @var array<string>
     */
    protected $fillable = [
        'name',
    ];
    
    /**
     * Relation with the accessories.
     */
    public function accessories()
    {
        return $this->hasMany(Accessory::class, 'id');
    }
}
